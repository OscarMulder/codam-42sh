/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   input_parse_ctrl_c.c                               :+:    :+:            */
/*                                                     +:+                    */
/*   By: tde-jong <tde-jong@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/07/29 13:54:20 by tde-jong       #+#    #+#                */
/*   Updated: 2019/07/29 15:05:52 by tde-jong      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"

int	input_parse_ctrl_c(t_inputdata *data, char **line)
{
	(void)data;
	(void)line;
	if (data->c == '\3')
	{
		ft_putchar('\n');
		return (FUNCT_SUCCESS);
	}
	return (FUNCT_FAILURE);
}
