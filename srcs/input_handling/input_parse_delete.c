/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   input_parse_delete.c                               :+:    :+:            */
/*                                                     +:+                    */
/*   By: omulder <omulder@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/05/16 13:44:53 by rkuijper       #+#    #+#                */
/*   Updated: 2019/08/27 18:59:04 by jbrinksm      ########   odam.nl         */
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

	if (data->line->index < data->line->len_cur)
	{
		ft_memcpy(&saved_coord, &data->curs->coords, sizeof(t_point));
		input_clear_char_at(&data->line->line, data->line->index);
		ft_putstr("\e[s");
		saved_index = data->line->index;
		curs_go_home(data);
		// ft_printf("\e[%iD", data->prompt->prompt_len);
		tc_clear_lines_str = tgetstr("cd", NULL);
		if (tc_clear_lines_str == NULL)
			return (FUNCT_ERROR);
		tputs(tc_clear_lines_str, 1, &ft_tputchar);
		// data->curs->coords.x = 1;
		// data->curs->coords.y = 1;
		// shell_display_prompt(data, data->prompt->cur_prompt_type);
		input_print_str(data, data->line->line);
		ft_putstr("\e[u");
		data->line->index = saved_index;
		data->line->len_cur--;
		ft_memcpy(&data->curs->coords, &saved_coord, sizeof(t_point));
	}
	return (FUNCT_SUCCESS);
}
