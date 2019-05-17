/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   input_parse_home.c                                 :+:    :+:            */
/*                                                     +:+                    */
/*   By: rkuijper <rkuijper@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/05/16 13:37:33 by rkuijper       #+#    #+#                */
/*   Updated: 2019/05/17 11:00:10 by rkuijper      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"

int				input_parse_home(char c, int *input_state, unsigned *index)
{
	if ((*input_state == 2 && c == 'H') || c == 1)
	{
		ft_printf("\e[%dD", *index);
		*index = 0;
		*input_state = 0;
		return (FUNCT_SUCCESS);
	}
	return (FUNCT_FAILURE);
}
