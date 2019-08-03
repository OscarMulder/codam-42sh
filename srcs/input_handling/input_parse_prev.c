/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   input_parse_prev.c                                 :+:    :+:            */
/*                                                     +:+                    */
/*   By: omulder <omulder@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/05/16 13:39:59 by rkuijper       #+#    #+#                */
/*   Updated: 2019/08/03 11:57:15 by rkuijper      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"

static void	parse_prev_move_word(t_vshdata *vshdata, unsigned *index)
{
	unsigned i;

	i = *index;
	if (*index == 0)
		return ;
	while (i > 0)
	{
		if (input_is_word_start(vshdata->line, i - 1, i) && i != *index)
			break ;
		i--;
	}
	input_move_to_index(index, i, vshdata);
}

int			input_parse_prev(t_inputdata *data, t_vshdata *vshdata)
{
	if (((data->input_state == INPUT_BRACE ||
	data->input_state == INPUT_D_BRACE) && data->c == 'D') ||
	(data->input_state == INPUT_ESC && data->c == 'b'))
	{
		if (data->input_state == INPUT_BRACE)
			input_move_to_index(&data->index, data->index - 1, vshdata);
		else
			parse_prev_move_word(vshdata, &data->index);
		data->input_state = INPUT_NONE;
		return (FUNCT_SUCCESS);
	}
	return (FUNCT_FAILURE);
}
