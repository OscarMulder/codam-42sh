/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   input_parse_backspace.c                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: rkuijper <rkuijper@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/05/16 13:43:07 by rkuijper       #+#    #+#                */
/*   Updated: 2019/08/03 14:42:54 by rkuijper      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"

int		input_parse_backspace(t_inputdata *data, t_vshdata *vshdata)
{
	unsigned len;

	if (data->c == INPUT_BACKSPACE)
	{
		if (data->index > 0)
		{
			input_clear_char_at(vshdata, data->index - 1);
			ft_printf("\e[D%s \e[D", vshdata->line + data->index - 1);
			len = ft_strlen(&(vshdata->line)[data->index - 1]);
			if (len > 1)
				ft_printf("\e[%dD", len);
			(data->index)--;
		}
		return (FUNCT_SUCCESS);
	}
	return (FUNCT_FAILURE);
}
