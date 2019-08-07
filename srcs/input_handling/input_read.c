/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   input_read.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: omulder <omulder@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/04/17 14:03:16 by jbrinksm       #+#    #+#                */
/*   Updated: 2019/08/07 11:23:40 by jbrinksm      ########   odam.nl         */
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

#include <sys/ioctl.h> //ADDED BY ME
#define TC_GETCURSORPOS "\e[6n" //ADDED BY ME
#define CURS_LEFT "\e[D"

static int	get_cursor_linepos()
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
	return ((short)ft_atoi(&response[i + 1]));
}

int			input_move_cursor_left(t_inputdata *data)
{
	struct winsize	ws;

	ioctl(STDIN_FILENO, TIOCGWINSZ, &ws);
	ft_eprintf("BEF LINEPOS: %i/%i\n", get_cursor_linepos(), ws.ws_col);
	if (data->index > 0)
	{
		if (get_cursor_linepos() == 1)
		{
			ft_putstr("\e[A");
			ft_printf("\e[%iC", ws.ws_col - 1);
		}
		else
			ft_putstr(CURS_LEFT);
		(data->index)--;
	}
	ft_eprintf("AFT LINEPOS: %i/%i\n", get_cursor_linepos(), ws.ws_col);
	return (FUNCT_SUCCESS);
}

int			input_move_cursor_right(t_inputdata *data, t_vshdata *vshdata)
{
	struct winsize	ws;
	int				strlen;

	ioctl(STDIN_FILENO, TIOCGWINSZ, &ws);
	strlen = ft_strlen(vshdata->line);
	ft_eprintf("BEF LINEPOS: %i/%i\n", get_cursor_linepos(), ws.ws_col);
	if (data->index > 0)
	{
		if (get_cursor_linepos() == 1)
		{
			ft_putstr("\e[A");
			ft_printf("\e[%iC", ws.ws_col - 1);
		}
		else
			ft_putstr(CURS_LEFT);
		(data->index)--;
	}
	ft_eprintf("AFT LINEPOS: %i/%i\n", get_cursor_linepos(), ws.ws_col);
	return (FUNCT_SUCCESS);
}

int			input_read(t_vshdata *vshdata)
{
	t_inputdata *data;
	int			char_status;
	
	data = init_inputdata(vshdata);
	if (data == NULL)
		return (FUNCT_ERROR);
	vshdata->line = ft_strnew(data->len_max);
	if (vshdata->line == NULL)
		return (ft_free_return(data, FUNCT_ERROR));
	
	while (read(STDIN_FILENO, &data->c, 1) > 0)
	{
		char_status = 0;
		char_status |= input_parse_escape(data);
		if (input_parse_ctrl_c(data) == FUNCT_SUCCESS)
			return (ft_free_return(data, NEW_PROMPT));
		ft_eprintf("%i %c|E\n", data->input_state, data->c);
		if (data->input_state == INPUT_BRACE && data->c == 'D')
			input_move_cursor_left(data);
		if (data->c == 'i')
		{
			ft_putstr("teststr");
			data->index += 7;
			data->input_state = INPUT_NONE;
		}
		if (data->c == '\n')
			builtin_exit(&vshdata->line, vshdata);
	}
	return (ft_free_return(data, FUNCT_ERROR));
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
