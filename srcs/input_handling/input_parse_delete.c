/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   input_parse_delete.c                               :+:    :+:            */
/*                                                     +:+                    */
/*   By: omulder <omulder@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/05/16 13:44:53 by rkuijper       #+#    #+#                */
/*   Updated: 2019/07/31 13:22:33 by omulder       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"

int	input_parse_delete(t_inputdata *data, char **line)
{
	if (data->input_state == INPUT_THREE && data->c == '~')
	{
		if (data->index < ft_strlen(*line))
		{
			input_clear_char_at(line, data->index);
			ft_printf("%s ", *line + data->index);
			ft_printf("\e[%dD", ft_strlen(*line + data->index) + 1);
		}
		else
			ft_putchar(7);
		data->input_state = INPUT_NONE;
		return (FUNCT_SUCCESS);
	}
	return (FUNCT_FAILURE);
}
