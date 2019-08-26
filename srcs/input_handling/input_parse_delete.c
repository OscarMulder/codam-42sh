/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   input_parse_delete.c                               :+:    :+:            */
/*                                                     +:+                    */
/*   By: omulder <omulder@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/05/16 13:44:53 by rkuijper       #+#    #+#                */
/*   Updated: 2019/08/26 12:47:04 by rkuijper      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"
#include <term.h>

/*
**	Real line gets updated, then the cursor position is saved (DOESNT WORK WITH RESIZING)
**	Lines will be cleared and everything will be reprinted (sadly).
*/

int			input_handle_delete(t_vshdata *data)
{
	char		*tc_clear_lines_str;
	unsigned	saved_index;
	t_point		saved_coord;

	if (data->index < data->len_cur)
	{
		ft_memcpy(&saved_coord, &data->coords, sizeof(t_point));
		input_clear_char_at(&vshdata->line, data->index);
		ft_putstr("\e[s");
		saved_index = data->index;
		curs_go_home(data, data);
		ft_printf("\e[%iD", data->prompt_len);
		tc_clear_lines_str = tgetstr("cd", NULL);
		if (tc_clear_lines_str == NULL)
			return (FUNCT_ERROR);
		tputs(tc_clear_lines_str, 1, &ft_tputchar);
		shell_display_prompt(data, data->cur_prompt_type);
		input_print_str(data, data->line);
		ft_putstr("\e[u");
		data->index = saved_index;
		data->len_cur--;
		ft_memcpy(&data->coords, &saved_coord, sizeof(t_point));
	}
	return (FUNCT_SUCCESS);
}
