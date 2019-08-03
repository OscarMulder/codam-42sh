/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   input_parse_end.c                                  :+:    :+:            */
/*                                                     +:+                    */
/*   By: rkuijper <rkuijper@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/05/16 13:39:04 by rkuijper       #+#    #+#                */
/*   Updated: 2019/08/03 11:55:13 by rkuijper      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"

int	input_parse_end(t_inputdata *data, t_vshdata *vshdata)
{
	if ((data->input_state == INPUT_BRACE && data->c == 'F') || data->c == '\5')
	{
		input_move_to_index(&data->index, ft_strlen(vshdata->line), vshdata);
		data->input_state = INPUT_NONE;
		return (FUNCT_SUCCESS);
	}
	return (FUNCT_FAILURE);
}
