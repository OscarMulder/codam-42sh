/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   input_parse_backspace.c                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: rkuijper <rkuijper@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/05/16 13:43:07 by rkuijper       #+#    #+#                */
/*   Updated: 2019/08/19 14:23:07 by jbrinksm      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"
#include <term.h>

/*
**	Backspaces are handled saving the cursor position and then clearing the
**	screen and then reprinting the edited line and then going back to the old
**	cursor position.
*/

void		input_handle_backspace(t_inputdata *data, t_vshdata *vshdata)
{
	char		*tc_clear_lines_str;
	unsigned	saved_index;

	if (data->index > 0)
	{
		input_clear_char_at(&vshdata->line, data->index - 1);
		ft_putstr("\e[s"); //save cursor pos
		saved_index = data->index; //save index
		
		curs_go_home(data);
		ft_printf("\e[%iD", vshdata->prompt_len);
		tc_clear_lines_str = tgetstr("cd", NULL);
		if (tc_clear_lines_str == NULL)
		{
			ft_eprintf("ERROR\n"); // DEBUG PRINT
			return ; // do fatal shit
		}
		tputs(tc_clear_lines_str, 1, &ft_tputchar);
		shell_display_prompt(vshdata, vshdata->cur_prompt_type);
		ft_putstr(vshdata->line);
		ft_putstr("\e[u"); //recover cursor pos
		data->index = saved_index; // recover index
		data->len_cur--;
		curs_move_left(data);
	}
}
