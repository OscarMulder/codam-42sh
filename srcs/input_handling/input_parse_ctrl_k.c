/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   input_parse_ctrl_k.c                               :+:    :+:            */
/*                                                     +:+                    */
/*   By: rkuijper <rkuijper@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/05/16 13:48:04 by rkuijper       #+#    #+#                */
/*   Updated: 2019/08/14 12:13:53 by rkuijper      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"
#include <term.h>

/*
**	Not finished yet, can be better....
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
		ft_printf("\e[1%dP", data->len_cur - data->index);
		data->len_cur -= (data->len_cur - data->index);
	}
	return (FUNCT_SUCCESS);
}
