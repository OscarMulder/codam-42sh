/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   input_parse_backspace.c                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: rkuijper <rkuijper@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/05/16 13:43:07 by rkuijper       #+#    #+#                */
/*   Updated: 2019/08/26 13:24:21 by rkuijper      ########   odam.nl         */
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
	int			len_left;
	int			saved_index;

	if (data->index > 0)
	{
		saved_index = data->index;
		len_left = data->len_cur - data->index;
		data->len_cur--;
		curs_go_home(data, vshdata);
		input_clear_char_at(&vshdata->line, saved_index - 1);
		ft_printf("\e[%iD", vshdata->prompt_len);
		tc_clear_lines_str = tgetstr("cd", NULL);
		if (tc_clear_lines_str == NULL)
			return ;
		tputs(tc_clear_lines_str, 1, &ft_tputchar);
		shell_display_prompt(vshdata, vshdata->cur_prompt_type);
		input_print_str(data, vshdata->line);
		data->index = data->len_cur;
		curs_move_n_left(data, vshdata, len_left);
	}
}
