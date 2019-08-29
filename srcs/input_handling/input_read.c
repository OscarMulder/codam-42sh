/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   input_read.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: omulder <omulder@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/04/17 14:03:16 by jbrinksm       #+#    #+#                */
/*   Updated: 2019/08/29 22:46:29 by jbrinksm      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"
#include <unistd.h>

void		input_clear_char_at(char **line, unsigned index)
{
	unsigned i;

	i = index;
	while ((*line)[i])
	{
		(*line)[i] = (*line)[i + 1];
		i++;
	}
}

static int	input_read_ansi_arrows(t_vshdata *data,
	char termcapbuf[TERMCAPBUFFSIZE])
{
	if (ft_strequ(termcapbuf, TC_LEFT_ARROW) == true)
		curs_move_left(data);
	else if (ft_strequ(termcapbuf, TC_RIGHT_ARROW) == true)
		curs_move_right(data);
	else if (ft_strequ(termcapbuf, TC_UP_ARROW) == true)
		history_change_line(data, ARROW_UP);
	else if (ft_strequ(termcapbuf, TC_DOWN_ARROW) == true)
		history_change_line(data, ARROW_DOWN);
	else if (ft_strequ(termcapbuf, TC_CTRL_RIGHT_ARROW) == true)
		curs_move_next_word(data);
	else if (ft_strequ(termcapbuf, TC_CTRL_LEFT_ARROW) == true)
		curs_move_prev_word(data);
	else if (ft_strequ(termcapbuf, TC_CTRL_UP_ARROW) == true)
		curs_move_up(data);
	else if (ft_strequ(termcapbuf, TC_CTRL_DOWN_ARROW) == true)
		curs_move_down(data);
	else
		return (FUNCT_FAILURE);
	return (FUNCT_SUCCESS);
}

/*
**	Is called after reading '\e'. The complete sequence is dumped into a buf.
**	If we support the escape sequence, it is handled by any of the functions
**	below, otherwise it is ignored.
*/

int			input_read_ansi(t_vshdata *data)
{
	char	termcapbuf[TERMCAPBUFFSIZE];

	ft_bzero(termcapbuf, TERMCAPBUFFSIZE);
	if (data->input->c == '\e')
	{
		termcapbuf[0] = '\e';
		if (read(STDIN_FILENO, &termcapbuf[1], TERMCAPBUFFSIZE - 1) == -1)
			return (FUNCT_ERROR);
		if (input_read_ansi_arrows(data, termcapbuf) != FUNCT_SUCCESS)
		{
			if (ft_strequ(termcapbuf, TC_HOME) == true)
				curs_go_home(data);
			else if (ft_strequ(termcapbuf, TC_END) == true)
				curs_go_end(data);
			else if (ft_strequ(termcapbuf, TC_DELETE) == true)
				input_handle_delete(data);
			else
			{
				#ifdef DEBUG
				ft_eprintf(">%s< TERMCAP NOT FOUND\n", &termcapbuf[1]); // DEBUG PRINT
				#endif
				return (FUNCT_FAILURE);
			}
		}
		return (FUNCT_SUCCESS);
	}
	return (FUNCT_FAILURE);
}

/*
**	Handles non-ansi single-byte special chars.
*/

int			input_read_special(t_vshdata *data)
{
	if (data->input->c == INPUT_BACKSPACE)
		input_handle_backspace(data);
	else if (data->input->c == INPUT_CTRL_D)
		input_parse_ctrl_d(data);
	else if (data->input->c == INPUT_CTRL_K)
		input_parse_ctrl_k(data);
	else if (data->input->c == INPUT_CTRL_U)
		input_parse_ctrl_u(data);
	else if (data->input->c == INPUT_CTRL_Y)
		input_parse_ctrl_y(data);
	else
		return (FUNCT_FAILURE);
	return (FUNCT_SUCCESS);
}

static int	reset_input_read_return(t_vshdata *data, int ret)
{
	data->input->c = '\0';
	data->line->index = 0;
	data->line->len_max = 64;
	data->line->len_cur = 0;
	data->curs->coords.x = 1;
	data->curs->coords.y = 1;
	//probably more shit?
	return (ret);
}

static int	input_parse(t_vshdata *data)
{
	if (input_parse_ctrl_c(data) == FUNCT_SUCCESS)
		return (reset_input_read_return(data, NEW_PROMPT));
	else if (input_read_ansi(data) == FUNCT_FAILURE)
	{
		if (input_read_special(data) == FUNCT_FAILURE)
		{
			if (input_parse_char(data) == FUNCT_ERROR)
				return (reset_input_read_return(data, FUNCT_ERROR));
		}
	}
	return (FUNCT_SUCCESS);
}

int			input_read(t_vshdata *data)
{
	if (data == NULL)
		return (FUNCT_ERROR);
	data->line->line = ft_strnew(data->line->len_max);
	if (data->line->line == NULL)
		return (reset_input_read_return(data, FUNCT_ERROR));
	while (true)
	{
		if (input_resize_window_check(data) == FUNCT_ERROR)
			return (reset_input_read_return(data, FUNCT_ERROR));
		if (read(STDIN_FILENO, &data->input->c, 1) == -1)
			return (reset_input_read_return(data, FUNCT_ERROR));
		if (input_parse(data) == NEW_PROMPT)
			return (NEW_PROMPT);
		if (data->input->c == '\n')
		{
			curs_go_end(data);
			break ;
		}
		data->input->c = '\0';
	}
	return (reset_input_read_return(data, FUNCT_SUCCESS));
}
