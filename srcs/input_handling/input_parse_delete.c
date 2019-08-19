/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   input_parse_delete.c                               :+:    :+:            */
/*                                                     +:+                    */
/*   By: omulder <omulder@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/05/16 13:44:53 by rkuijper       #+#    #+#                */
/*   Updated: 2019/08/19 14:18:49 by jbrinksm      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"
#include <term.h>

/*
**	Real line gets updated, then the cursor position is saved (DOESNT WORK WITH RESIZING)
**	Lines will be cleared and everything will be reprinted (sadly).
*/

int			input_handle_delete(t_inputdata *data, t_vshdata *vshdata)
{
	char		*tc_clear_lines_str;
	unsigned	saved_index;

	input_clear_char_at(&vshdata->line, data->index);
	ft_putstr("\e[s"); //save cursor pos
	saved_index = data->index; //save index
	curs_go_home(data);
	ft_printf("\e[%iD", vshdata->prompt_len);
	tc_clear_lines_str = tgoto(tgetstr("dc", NULL), 0, 1);
	if (tc_clear_lines_str == NULL)
	{
		ft_eprintf("ERROR\n"); // DEBUG PRINT
		return (FUNCT_ERROR); // do fatal shit
	}
	tputs(tc_clear_lines_str, 20, &ft_tputchar);
	shell_display_prompt(vshdata, vshdata->cur_prompt_type);
	ft_putstr(vshdata->line);
	ft_putstr("\e[u"); //recover cursor pos
	data->index = saved_index; // recover index
	data->len_cur--;
	return (FUNCT_SUCCESS);
}
