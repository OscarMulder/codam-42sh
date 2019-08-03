/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   input_parse_next.c                                 :+:    :+:            */
/*                                                     +:+                    */
/*   By: omulder <omulder@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/05/16 13:41:00 by rkuijper       #+#    #+#                */
/*   Updated: 2019/08/03 14:42:40 by rkuijper      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"

static void	parse_next_move_word(t_vshdata *vshdata, unsigned *index)
{
	unsigned i;

	i = *index + 1;
	if (*index == ft_strlen(vshdata->line))
		return ;
	while (vshdata->line[i])
	{
		if (input_is_word_start(vshdata->line, i - 1, i))
			break ;
		i++;
	}
	input_move_to_index(index, i, vshdata);
}

int			input_parse_next(t_inputdata *data, t_vshdata *vshdata)
{
	if (((data->input_state == INPUT_BRACE ||
	data->input_state == INPUT_D_BRACE) && data->c == 'C') ||
	(data->input_state == INPUT_ESC && data->c == 'f'))
	{
		if (data->input_state == INPUT_BRACE)
			input_move_to_index(&data->index, data->index + 1, vshdata);
		else
			parse_next_move_word(vshdata, &data->index);
		data->input_state = INPUT_NONE;
		return (FUNCT_SUCCESS);
	}
	return (FUNCT_FAILURE);
}
