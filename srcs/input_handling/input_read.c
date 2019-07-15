/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   input_read.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: jbrinksm <jbrinksm@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/04/17 14:03:16 by jbrinksm       #+#    #+#                */
/*   Updated: 2019/07/15 17:06:13 by omulder       ########   odam.nl         */
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

t_inputdata	*init_inputdata(void)
{
	t_inputdata	*new;

	new = (t_inputdata*)ft_memalloc(sizeof(t_inputdata));
	new->c = 0;
	new->index = 0;
	new->input_state = 0;
	new->hist_index = 0;
	return (new);
}

int			input_read(char **line, t_history **history)
{
	t_inputdata	*data;
	int			status;
	int			maxstrlen;

	data = init_inputdata();
	*line = ft_strnew(64);
	maxstrlen = 64;
	while (read(STDIN_FILENO, &data->c, 1) > 0)
	{
		status = 0;
		status |= input_parse_escape(data);
		status |= input_parse_home(data);
		status |= input_parse_end(data, line);
		status |= input_parse_prev(data, line);
		status |= input_parse_next(data, line);
		status |= input_parse_delete(data, line);
		status |= input_parse_ctrl_up(data, history, line);
		status |= input_parse_ctrl_down(data, history, line);
		if (status == 0)
			data->input_state = 0;
		status |= input_parse_backspace(data, line);
		status |= input_parse_ctrl_d(data, line);
		status |= input_parse_ctrl_k(data, line);
		if (status == 0 && input_parse_char(data, line, &maxstrlen) == FUNCT_FAILURE)
			return (FUNCT_FAILURE);
		if (data->c == '\n')
			break ;
	}
	return (status);
}
