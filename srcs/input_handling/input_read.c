/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   input_read.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: jbrinksm <jbrinksm@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/04/17 14:03:16 by jbrinksm       #+#    #+#                */
/*   Updated: 2019/07/26 16:11:01 by omulder       ########   odam.nl         */
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

int			input_read(t_vshdata *vshdata, char **line, int *status)
{
	t_inputdata	*data;
	int			local_status;
	int			len_max;

	data = init_inputdata();
	len_max = 64;
	*line = ft_strnew(len_max);
	if (*line == NULL)
		return (FUNCT_ERROR);
	while (read(STDIN_FILENO, &data->c, 1) > 0)
	{
		local_status = 0;
		local_status |= input_parse_escape(data);
		local_status |= input_parse_home(data);
		local_status |= input_parse_end(data, line);
		local_status |= input_parse_prev(data, line);
		local_status |= input_parse_next(data, line);
		local_status |= input_parse_delete(data, line);
		local_status |= input_parse_ctrl_up(data, vshdata->history, line);
		local_status |= input_parse_ctrl_down(data, vshdata->history, line);
		if (local_status == 0)
			data->input_state = 0;
		local_status |= input_parse_backspace(data, line);
		local_status |= input_parse_ctrl_d(data, line);
		local_status |= input_parse_ctrl_k(data, line);
		if (local_status == 0 && input_parse_char(data, line, &len_max) == FUNCT_ERROR)
			return (FUNCT_ERROR);
		if (data->c == '\n')
			break ;
	}
	*status = local_status;
	return (FUNCT_SUCCESS);
}
