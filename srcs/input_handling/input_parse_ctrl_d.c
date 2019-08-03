/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   input_parse_ctrl_d.c                               :+:    :+:            */
/*                                                     +:+                    */
/*   By: omulder <omulder@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/05/16 13:46:55 by rkuijper       #+#    #+#                */
/*   Updated: 2019/08/03 14:42:51 by rkuijper      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"

int	input_parse_ctrl_d(t_inputdata *data, t_vshdata *vshdata)
{
	unsigned	len;

	if (data->c == '\4')
	{
		len = ft_strlen(vshdata->line);
		if (len == 0)
			builtin_exit(NULL, vshdata);
		if (data->index < len)
		{
			input_clear_char_at(vshdata, data->index);
			ft_printf("%s ", vshdata->line + data->index);
			ft_printf("\e[%dD", ft_strlen(vshdata->line + data->index) + 1);
		}
		return (FUNCT_SUCCESS);
	}
	return (FUNCT_FAILURE);
}
