/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   input_parse_ctrl_c.c                               :+:    :+:            */
/*                                                     +:+                    */
/*   By: omulder <omulder@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/07/29 13:54:20 by tde-jong       #+#    #+#                */
/*   Updated: 2019/08/07 22:26:16 by jbrinksm      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"
#include <term.h>

int	input_parse_ctrl_c(t_inputdata *data, t_vshdata *vshdata)
{
	if (data->c == INPUT_CTRL_C)
	{
		curs_go_end(data, vshdata);
		ft_putchar('\n');
		return (FUNCT_SUCCESS);
	}
	return (FUNCT_FAILURE);
}
