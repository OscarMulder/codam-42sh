/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   input_parse_delete.c                               :+:    :+:            */
/*                                                     +:+                    */
/*   By: omulder <omulder@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/05/16 13:44:53 by rkuijper       #+#    #+#                */
/*   Updated: 2019/08/08 15:28:30 by jbrinksm      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"
#include <term.h>

// int	input_parse_delete(t_inputdata *data, char **line)
// {
// 	if (data->input_state == INPUT_THREE && data->c == '~')
// 	{
// 		if (data->index < ft_strlen(*line))
// 		{
// 			input_clear_char_at(line, data->index);
// 			ft_printf("%s ", *line + data->index);
// 			ft_printf("\e[%dD", ft_strlen(*line + data->index) + 1);
// 		}
// 		else
// 			ft_putchar('\a');
// 		data->input_state = INPUT_NONE;
// 		return (FUNCT_SUCCESS);
// 	}
// 	return (FUNCT_FAILURE);
// }

int			input_handle_delete(t_inputdata *data, t_vshdata *vshdata)
{
	char		*tc_clear_lines_str;
	unsigned	saved_index;

	input_clear_char_at(&vshdata->line, data->index);
	ft_putstr("\e[s"); //save cursor pos
	saved_index = data->index; //save index
	
	curs_go_home(data, vshdata);
	ft_printf("\e[%iD", vshdata->prompt_len);
	tc_clear_lines_str = tgetstr("cd", NULL);
	if (tc_clear_lines_str == NULL)
	{
		ft_eprintf("ERROR\n"); // DEBUG PRINT
		return (FUNCT_ERROR); // do fatal shit
	}
	tputs(tc_clear_lines_str, 1, &ft_tputchar);
	shell_display_prompt(vshdata);
	ft_putstr(vshdata->line);
	ft_putstr("\e[u"); //recover cursor pos
	data->index = saved_index; // recover index
	return (FUNCT_SUCCESS);
}
