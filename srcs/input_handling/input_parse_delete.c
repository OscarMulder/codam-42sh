/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   input_parse_delete.c                               :+:    :+:            */
/*                                                     +:+                    */
/*   By: omulder <omulder@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/05/16 13:44:53 by rkuijper       #+#    #+#                */
/*   Updated: 2019/08/01 16:56:00 by rkuijper      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"

/* TODO
	Fix a newline deletion to visually remove the last line.
	Also fix deletion before a newline to fill newly found spaces with.. Spaces.
*/
int	input_parse_delete(t_inputdata *data, t_vshdata *vshdata, char **line)
{
	int i;
	
	if (data->input_state == INPUT_THREE && data->c == '~')
	{
		if (data->index < ft_strlen(*line))
		{
			input_clear_char_at(line, data->index);
			ft_printf("%s ", *line + data->index);
			i = ft_strlen(*line + data->index) + 1;
			data->index += i;
			input_move_to_index(&data->index, data->index - i, *line, vshdata);
		}
		else
			ft_putchar('\a');
		data->input_state = INPUT_NONE;
		return (FUNCT_SUCCESS);
	}
	return (FUNCT_FAILURE);
}
