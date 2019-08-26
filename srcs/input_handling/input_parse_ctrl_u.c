/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   input_parse_ctrl_u.c                               :+:    :+:            */
/*                                                     +:+                    */
/*   By: rkuijper <rkuijper@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/08/23 12:15:47 by rkuijper       #+#    #+#                */
/*   Updated: 2019/08/26 18:36:17 by jbrinksm      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"
#include <term.h>

int			input_parse_ctrl_u(t_vshdata *data)
{
	char		*tc_clear_lines_str;
	
	if (data->line->index > 0)
	{
		if (data->line->line_copy != NULL)
			ft_strdel(&data->line->line_copy);
		data->line->line_copy = ft_strndup(data->line->line, data->line->index);
		ft_memcpy(data->line->line, &data->line->line[data->line->index], data->line->index);
		ft_bzero(&data->line->line[data->line->index], data->line->len_cur - data->line->index);
		curs_go_home(data);
		ft_printf("\e[%iD", data->prompt->prompt_len);
		tc_clear_lines_str = tgetstr("cd", NULL);
		if (tc_clear_lines_str == NULL)
		{
			ft_eprintf("ERROR\n"); // DEBUG PRINT
			return (FUNCT_ERROR); // do fatal shit
		}
		tputs(tc_clear_lines_str, 20, &ft_tputchar);
		shell_display_prompt(data, data->prompt->cur_prompt_type);		
		data->line->len_cur = ft_strlen(data->line->line);
		data->line->index = data->line->len_cur;
		input_print_str(data, data->line->line);
		curs_go_home(data);
	}
	return (FUNCT_SUCCESS);
}
