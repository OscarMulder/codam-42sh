/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   input_parse_home.c                                 :+:    :+:            */
/*                                                     +:+                    */
/*   By: omulder <omulder@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/05/16 13:37:33 by rkuijper       #+#    #+#                */
/*   Updated: 2019/08/03 11:55:50 by rkuijper      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"

int	input_parse_home(t_inputdata *data, t_vshdata *vshdata)
{
	if ((data->input_state == INPUT_BRACE && data->c == 'H') || data->c == '\1')
	{
		input_move_to_index(&data->index, 0, vshdata);
		data->input_state = INPUT_NONE;
		return (FUNCT_SUCCESS);
	}
	return (FUNCT_FAILURE);
}
