/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   input_parse_ctrl_k.c                               :+:    :+:            */
/*                                                     +:+                    */
/*   By: rkuijper <rkuijper@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/05/16 13:48:04 by rkuijper       #+#    #+#                */
/*   Updated: 2019/08/12 14:29:46 by rkuijper      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"
#include <term.h>

/*
**
**	This is not using any ncurses functionality, since that didn't wanna work...
**
*/

int			input_parse_ctrl_k(t_inputdata *data, t_vshdata *vshdata)
{
	unsigned i;
	
	if (data->index < data->len_cur)
	{
		i = data->index;
		while (i < data->len_cur)
		{
			vshdata->line[i] = '\0';
			i++;
		}
		ft_printf("\e[2%dP", data->len_cur - data->index);
		data->len_cur -= (data->len_cur - data->index);
	}
	return (FUNCT_SUCCESS);
}
