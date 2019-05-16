/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   input_parse_escape.c                               :+:    :+:            */
/*                                                     +:+                    */
/*   By: rkuijper <rkuijper@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/05/16 13:36:25 by rkuijper       #+#    #+#                */
/*   Updated: 2019/05/16 13:58:08 by rkuijper      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"

int				input_parse_escape(char c, int *input_state)
{
	if (*input_state == 0 && c == 27)
	{
		*input_state = 1;
		return (1);
	}
	else if (*input_state == 1 && c == 27)
	{
		*input_state = 4;
		return (1);
	}
	else if ((*input_state == 1 || *input_state == 4) && c == 91)
	{
		(*input_state)++;
		return (1);
	}
	else if ((*input_state == 2 || *input_state == 5) && c == 51)
	{
		(*input_state)++;
		return (1);
	}
	return (0);
}
