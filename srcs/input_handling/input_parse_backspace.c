/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   input_parse_backspace.c                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: rkuijper <rkuijper@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/05/16 13:43:07 by rkuijper       #+#    #+#                */
/*   Updated: 2019/08/03 16:15:38 by rkuijper      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"

/*
**
**	Parses backspace input.
**		- First checks if input is valid for backspace interception.
**		- Calls 'input_clear_char_at' to remove one character from the buffer.
**		- Uses 'ft_printf' to move one space back (\e[D) in the visual buffer,
**			then print the string (%s) and move one space back again (\e[D).
**		- After that, the complete string had been printed; the cursor
**			needs to be positioned way back, hence a `\e%dD` print with len.
*/

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
