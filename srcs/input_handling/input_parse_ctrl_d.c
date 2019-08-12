/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   input_parse_ctrl_d.c                               :+:    :+:            */
/*                                                     +:+                    */
/*   By: omulder <omulder@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/05/16 13:46:55 by rkuijper       #+#    #+#                */
/*   Updated: 2019/08/12 14:29:03 by rkuijper      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"

/*
**	Simply exits when line is empty, otherwise functions as `delete`.
*/

int	input_parse_ctrl_d(t_inputdata *data, t_vshdata *vshdata)
{
	unsigned	len;

	if (data->c == INPUT_CTRL_D)
	{
		len = data->len_cur;
		if (len == 0)
			builtin_exit(NULL, vshdata);
		if (data->index < len)
			input_handle_delete(data, vshdata);
		return (FUNCT_SUCCESS);
	}
	return (FUNCT_FAILURE);
}
