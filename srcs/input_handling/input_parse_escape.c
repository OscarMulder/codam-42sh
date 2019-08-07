/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   input_parse_escape.c                               :+:    :+:            */
/*                                                     +:+                    */
/*   By: rkuijper <rkuijper@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/05/16 13:36:25 by rkuijper       #+#    #+#                */
/*   Updated: 2019/08/07 10:18:59 by jbrinksm      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"

int	input_parse_escape(t_inputdata *data)
{
	if (data->input_state == INPUT_NONE && data->c == '\e')
	{
		ft_eprintf("ESC RECIEVED\n");
		data->input_state = INPUT_ESC;
	}
	else if (data->input_state == INPUT_ESC && data->c == '\e')
	{
		ft_eprintf("DESC RECIEVED\n");
		// data->input_state = INPUT_D_ESC;
		data->input_state = INPUT_ESC;
	}
	else if (data->input_state == INPUT_ESC && data->c == '[')
	{
		ft_eprintf("LBRACE RECEIVED\n");
		(data->input_state)++;
	}
	else if (data->input_state == INPUT_BRACE && data->c == '3')
	{
		ft_eprintf("3 RECIEVED\n");
		(data->input_state)++;
	}
	else
	{
		ft_eprintf("%c RECEIVED\n", data->c);
		return (FUNCT_FAILURE);
	}
	return (FUNCT_SUCCESS);
}
