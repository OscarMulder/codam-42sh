/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   input_parse_ctrl_d.c                               :+:    :+:            */
/*                                                     +:+                    */
/*   By: omulder <omulder@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/05/16 13:46:55 by rkuijper       #+#    #+#                */
/*   Updated: 2019/08/07 22:17:29 by jbrinksm      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"

int	input_parse_ctrl_d(t_inputdata *data, t_vshdata *vshdata)
{
	unsigned	len;

	if (data->c == INPUT_CTRL_D)
	{
		len = ft_strlen(vshdata->line);
		if (len == 0)
			builtin_exit(NULL, vshdata);
		if (data->index < len)
			input_handle_delete(data, vshdata);
		return (FUNCT_SUCCESS);
	}
	return (FUNCT_FAILURE);
}
