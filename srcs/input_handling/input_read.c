/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   input_read.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: omulder <omulder@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/04/17 14:03:16 by jbrinksm       #+#    #+#                */
/*   Updated: 2019/09/17 15:11:20 by jbrinksm      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"
#include <unistd.h>
#include <signal.h>

void		input_reset_cursor_pos(void)
{
	size_t		i;
	int			output;
	size_t		answer_len;
	char		answer[TC_MAXRESPONSESIZE];

	answer_len = 0;
	write(STDIN_FILENO, TC_GETCURSORPOS, 4);
	while (answer_len < sizeof(answer) - 1 &&
		read(1, answer + answer_len, 1) == 1)
	{
		if (answer[answer_len] == 'R')
			break ;
		answer_len++;
	}
	answer[answer_len] = '\0';
	i = 1;
	while (i < answer_len && answer[i] != ';')
		i++;
	if (answer[i] != '\0')
	{
		i++;
		output = ft_atoi(&answer[i]);
		if (output > 1)
			ft_putstr("\e[47;30m%\e[0m\n");
	}
}

static int	find_start(t_history **history)
{
	int i;
	int start;
	int largest;

	i = 0;
	start = 0;
	largest = -1;
	while (i < HISTORY_MAX && history[i]->str != NULL)
	{
		if (history[i]->number > largest)
		{
			start = i;
			largest = history[i]->number;
		}
		i++;
	}
	return (start + 1);
}

static int	reset_input_read_return(t_vshdata *data, int ret)
{
	data->input->c = '\0';
	data->line->index = 0;
	data->line->len_max = 64;
	data->line->len_cur = 0;
	data->curs->coords.x = data->prompt->prompt_len + 1;
	data->curs->coords.y = get_curs_row();
	data->curs->cur_relative_y = 1;
	data->history->hist_index = find_start(data->history->history);
	data->history->hist_start = data->history->hist_index - 1;
	data->history->hist_first = true;
	signal(SIGWINCH, SIG_DFL);
	return (ret);
}

static int	input_parse(t_vshdata *data)
{
	int		ret;

	if (input_parse_ctrl_c(data) == FUNCT_SUCCESS)
		return (reset_input_read_return(data, NEW_PROMPT));
	ret = input_parse_ctrl_d(data);
	if (ret == NEW_PROMPT)
		return (reset_input_read_return(data, NEW_PROMPT));
	else if (ret == IR_EOF)
		return (IR_EOF);
	else if (ret == FUNCT_SUCCESS)
		return (FUNCT_SUCCESS);
	else if (input_read_ansi(data) == FUNCT_FAILURE)
	{
		if (input_parse_special(data) == FUNCT_FAILURE)
		{
			if (input_parse_char(data) == FUNCT_ERROR)
				return (reset_input_read_return(data, FUNCT_ERROR));
		}
	}
	return (FUNCT_SUCCESS);
}

int			input_read(t_vshdata *data)
{
	int		ret;

	if (data == NULL)
		return (FUNCT_ERROR);
	data->line->line = ft_strnew(data->line->len_max);
	if (data->line->line == NULL)
		return (reset_input_read_return(data, FUNCT_ERROR));
	reset_input_read_return(data, 0);
	resize_window_check(SIGWINCH);
	term_disable_isig(data->term->termios_p);
	while (true)
	{
		if (read(STDIN_FILENO, &data->input->c, 1) == -1)
			return (reset_input_read_return(data, FUNCT_ERROR));
		ret = input_parse(data);
		if (ret == NEW_PROMPT || ret == FUNCT_ERROR || ret == IR_EOF)
			return (ret);
		if (data->input->c == '\n')
		{
			curs_go_end(data);
			break ;
		}
	}
	term_enable_isig(data->term->termios_p);
	return (reset_input_read_return(data, FUNCT_SUCCESS));
}
