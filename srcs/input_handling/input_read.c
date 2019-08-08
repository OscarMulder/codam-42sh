/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   input_read.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: omulder <omulder@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/04/17 14:03:16 by jbrinksm       #+#    #+#                */
/*   Updated: 2019/08/08 16:34:45 by jbrinksm      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"
#include <unistd.h>
#include <sys/ioctl.h> // REMOVE
#include <term.h> //remove

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
	int largest;
	int start;

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
	new->input_state = 0;
	new->hist_index = find_start(vshdata->history);
	new->hist_start = new->hist_index - 1;
	new->hist_first = true;
	new->history = vshdata->history;
	new->len_max = 64;
	return (new);
}

static char	*get_cursor_pos(void)
{
	char	*buf;
	int		ret;

	buf = ft_strnew(100);
	ret = read(STDIN_FILENO, buf, 10);
	if (ret == -1)
	{
		ft_strdel(&buf);
		return (NULL);
	}
	return (buf);
}

int		get_cursor_linepos(void)
{
	char	*response;
	int		i;

	ft_putstr(TC_GETCURSORPOS);
	response = get_cursor_pos();
	if (response == NULL)
		return (-1);
	i = 0;
	while (response[i] != '\0' && response[i] != ';')
		i++;
	if (response[i] == '\0' || response[i + 1] == '\0'
		|| ft_isdigit(response[i + 1]) == false)
	{
		ft_strdel(&response);
		return (-1);
	}
	return ((short)ft_atoi(&response[i + 1]) /*needs unsigned atoi and short  ret */);
}

int			input_read_ansi(t_inputdata *data, t_vshdata *vshdata)
{
	char	*termcapbuf;

	if (data->c == '\e')
	{
		termcapbuf = ft_strnew(12);
		if (termcapbuf == NULL)
		{
			// do fatal shit
			return (FUNCT_ERROR);
		}
		*termcapbuf = '\e';
		if (read(STDIN_FILENO, &termcapbuf[1], TERMCAPBUFFSIZE - 1) == -1)
		{
			// do fatal shit
			ft_strdel(&termcapbuf);
			return (FUNCT_ERROR);
		}
		if (ft_strequ(termcapbuf, TC_LEFT_ARROW) == true)
			curs_move_left(data);
		else if (ft_strequ(termcapbuf, TC_RIGHT_ARROW) == true)
			curs_move_right(data, vshdata->line);
		else if (ft_strequ(termcapbuf, TC_HOME) == true)
			curs_go_home(data, vshdata);
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
			ft_strdel(&termcapbuf);
			return (FUNCT_FAILURE);
		}
		ft_strdel(&termcapbuf);
		return (FUNCT_SUCCESS);
	}
	return (FUNCT_FAILURE);
}


int			input_read_special(t_inputdata *data, t_vshdata *vshdata)
{
	if (data->c == INPUT_BACKSPACE)
		input_handle_backspace(data, vshdata);
	else if (data->c == INPUT_CTRL_D)
		input_parse_ctrl_d(data, vshdata);
	else
		return (FUNCT_FAILURE);
	return (FUNCT_SUCCESS);
}

int			input_read(t_vshdata *vshdata /*will need ws.ws_col backup and cursor backup x and y*/)
{
	t_inputdata *data;

	data = init_inputdata(vshdata);
	if (data == NULL)
		return (FUNCT_ERROR);
	vshdata->line = ft_strnew(data->len_max);
	if (vshdata->line == NULL)
		return (ft_free_return(data, FUNCT_ERROR));
	while (true)
	{
		// get and compare new ws.ws_col
		// ft_eprintf("BEF: index: >%i<\n", data->index);
		if (read(STDIN_FILENO, &data->c, 1) == -1)
			return (ft_free_return(data, FUNCT_ERROR));
		if (input_parse_ctrl_c(data, vshdata) == FUNCT_SUCCESS)
			return (ft_free_return(data, NEW_PROMPT));
		else if (input_read_ansi(data, vshdata) == FUNCT_FAILURE)
		{
			if (input_read_special(data, vshdata) == FUNCT_FAILURE)
			{
				if (input_parse_char(data, &vshdata->line) == FUNCT_ERROR)
					return (ft_free_return(data, FUNCT_ERROR));
				if (data->c == '\n')
					break ;
			}
		}
		ft_eprintf("AFT: index: %i/%i\n", data->index, ft_strlen(vshdata->line)); // DEBUG PRINT
	}
	return (ft_free_return(data, FUNCT_SUCCESS));
}































// int			input_read(t_vshdata *vshdata, char **line, int *status)
// {
// 	t_inputdata	*data;
// 	int			local_status;

// 	data = init_inputdata(vshdata);
// 	if (data == NULL)
// 		return (FUNCT_ERROR);
// 	*line = ft_strnew(data->len_max);
// 	if (*line == NULL)
// 		return (ft_free_return(data, FUNCT_ERROR));
// 	while (read(STDIN_FILENO, &data->c, 1) > 0)
// 	{
// 		local_status = 0;
// 		local_status |= input_parse_escape(data);
// 		local_status |= input_parse_home(data);
// 		local_status |= input_parse_end(data, line);
// 		local_status |= input_parse_prev(data, line);
// 		local_status |= input_parse_next(data, line);
// 		local_status |= input_parse_delete(data, line);
// 		local_status |= input_parse_ctrl_up(data, line);
// 		local_status |= input_parse_ctrl_down(data, line);
// 		if (local_status == 0)
// 			data->input_state = 0;
// 		local_status |= input_parse_backspace(data, line);
// 		if (input_parse_ctrl_c(data) == FUNCT_SUCCESS)
// 			return (ft_free_return(data, NEW_PROMPT));
// 		local_status |= input_parse_ctrl_d(data, vshdata, line);
// 		local_status |= input_parse_ctrl_k(data, line);
// 		if (local_status == 0 && input_parse_char(data, line) == FUNCT_ERROR)
// 			return (ft_free_return(data, FUNCT_ERROR));
// 		if (data->c == '\n')
// 			break ;
// 	}
// 	*status = local_status;
// 	return (ft_free_return(data, FUNCT_SUCCESS));
// }
