/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   input_parse_home.c                                 :+:    :+:            */
/*                                                     +:+                    */
/*   By: omulder <omulder@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/05/16 13:37:33 by rkuijper       #+#    #+#                */
/*   Updated: 2019/07/31 12:33:42 by omulder       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"

/*
** The else if condition is to prevent the cursor to go over the 'prompt' part
** (vsh >) of the terminal line, while also preventing other input functions
** from executing the keypress.
*/

int	input_parse_home(t_inputdata *data)
{
	if (data->index > 0 &&
	((data->input_state == INPUT_BRACE && data->c == 'H') || data->c == '\1'))
	{
		ft_printf("\e[%dD", data->index);
		data->index = 0;
		data->input_state = INPUT_NONE;
		return (FUNCT_SUCCESS);
	}
	else if (data->index == 0 &&
	((data->input_state == INPUT_BRACE && data->c == 'H') || data->c == '\1'))
	{
		data->input_state = INPUT_NONE;
		return (FUNCT_SUCCESS);
	}
	return (FUNCT_FAILURE);
}
