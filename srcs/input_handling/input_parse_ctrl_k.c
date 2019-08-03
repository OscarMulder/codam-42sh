/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   input_parse_ctrl_k.c                               :+:    :+:            */
/*                                                     +:+                    */
/*   By: rkuijper <rkuijper@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/05/16 13:48:04 by rkuijper       #+#    #+#                */
/*   Updated: 2019/08/03 11:53:42 by rkuijper      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"

int	input_parse_ctrl_k(t_inputdata *data, t_vshdata *vshdata)
{
	unsigned i;

	if (data->c == '\v')
	{
		if (data->index < ft_strlen(vshdata->line))
		{
			i = (data->index);
			while (vshdata->line[i])
			{
				ft_putchar(' ');
				vshdata->line[i] = '\0';
				i++;
			}
			ft_printf("\e[%dD", i - data->index);
		}
		return (FUNCT_SUCCESS);
	}
	return (FUNCT_FAILURE);
}
