/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   input_read.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: omulder <omulder@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/04/17 14:03:16 by jbrinksm       #+#    #+#                */
/*   Updated: 2019/08/23 14:53:32 by rkuijper      ########   odam.nl         */
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

t_inputdata	*init_inputdata(t_vshdata *vshdata)
{
	t_inputdata	*new;

	new = (t_inputdata*)ft_memalloc(sizeof(t_inputdata));
	if (new == NULL)
		return (NULL);
	new->c = 0;
	new->index = 0;
	new->len_cur = 0;
	new->len_max = 64;
	new->coords = (t_point){ 1 + vshdata->prompt_len, 1 };
	new->hist_index = find_start(vshdata->history);
	new->hist_start = new->hist_index - 1;
	new->hist_first = true;
	new->history = vshdata->history;
	new->cur_ws_col = -1;
	return (new);
}

/*
**	Is called after reading '\e'. The complete sequence is dumped into a buf.
**	If we support the escape sequence, it is handled by any of the functions
**	below, otherwise it is ignored.
*/

int			input_read_ansi(t_inputdata *data, t_vshdata *vshdata)
{
	char	termcapbuf[TERMCAPBUFFSIZE];

	ft_bzero(termcapbuf, TERMCAPBUFFSIZE);
	if (data->c == '\e')
	{
		termcapbuf[0] = '\e';
		if (read(STDIN_FILENO, &termcapbuf[1], TERMCAPBUFFSIZE - 1) == -1)
			return (FUNCT_ERROR);
		if (ft_strequ(termcapbuf, TC_LEFT_ARROW) == true)
			curs_move_left(data);
		else if (ft_strequ(termcapbuf, TC_RIGHT_ARROW) == true)
			curs_move_right(data, vshdata);
		else if (ft_strequ(termcapbuf, TC_HOME) == true)
			curs_go_home(data);
		else if (ft_strequ(termcapbuf, TC_END) == true)
			curs_go_end(data, vshdata);
		else if (ft_strequ(termcapbuf, TC_DELETE) == true)
			input_handle_delete(data, vshdata);
		else if (ft_strequ(termcapbuf, TC_UP_ARROW) == true)
			history_change_line(data, vshdata, ARROW_UP);
		else if (ft_strequ(termcapbuf, TC_DOWN_ARROW) == true)
			history_change_line(data, vshdata, ARROW_DOWN);
		else if (ft_strequ(termcapbuf, TC_CTRL_RIGHT_ARROW) == true)
			curs_move_next_word(data, vshdata);
		else if (ft_strequ(termcapbuf, TC_CTRL_LEFT_ARROW) == true)
			curs_move_prev_word(data, vshdata);
		else if (ft_strequ(termcapbuf, TC_CTRL_UP_ARROW) == true)
			curs_move_up(data, vshdata);
		else if (ft_strequ(termcapbuf, TC_CTRL_DOWN_ARROW) == true)
			curs_move_down(data, vshdata);
		else
		{
			ft_eprintf(">%s< TERMCAP NOT FOUND\n", &termcapbuf[1]); // DEBUG PRINT
			return (FUNCT_FAILURE);
		}
		return (FUNCT_SUCCESS);
	}
	return (FUNCT_FAILURE);
}

/*
**	Handles non-ansi single-byte special chars.
*/

int			input_read_special(t_inputdata *data, t_vshdata *vshdata)
{
	if (data->c == INPUT_BACKSPACE)
		input_handle_backspace(data, vshdata);
	else if (data->c == INPUT_CTRL_D)
		input_parse_ctrl_d(data, vshdata);
	else if (data->c == INPUT_CTRL_K)
		input_parse_ctrl_k(data, vshdata);
	else if (data->c == INPUT_CTRL_U)
		input_parse_ctrl_u(data, vshdata);
	else if (data->c == INPUT_CTRL_Y)
		input_parse_ctrl_y(data, vshdata);
	else
		return (FUNCT_FAILURE);
	return (FUNCT_SUCCESS);
}

int			input_read(t_vshdata *vshdata /*will need ws.ws_col backup and cursor backup x and y*/)
{
	t_inputdata *data;
	int			ret;

	data = init_inputdata(vshdata);
	if (data == NULL)
		return (FUNCT_ERROR);
	vshdata->line = ft_strnew(data->len_max);
	if (vshdata->line == NULL)
		return (ft_free_return(data, FUNCT_ERROR));
	while (true)
	{
		if (read(STDIN_FILENO, &data->c, 1) == -1)
			return (ft_free_return(data, FUNCT_ERROR));
		else if (ret == 0)
			continue ;
		if (input_parse_ctrl_c(data, vshdata) == FUNCT_SUCCESS)
			return (ft_free_return(data, NEW_PROMPT));
		else if (input_read_ansi(data, vshdata) == FUNCT_FAILURE)
		{
			if (input_read_special(data, vshdata) == FUNCT_FAILURE)
			{
				if (input_parse_char(data, vshdata) == FUNCT_ERROR)
					return (ft_free_return(data, FUNCT_ERROR));
				if (data->c == '\n')
				{
					curs_go_end(data, vshdata);
					break ;
				}
			}
		}
		ft_eprintf("AFT: index: %i/%i\n", data->index, data->len_cur); // DEBUG PRINT
	}
	return (ft_free_return(data, FUNCT_SUCCESS));
}
