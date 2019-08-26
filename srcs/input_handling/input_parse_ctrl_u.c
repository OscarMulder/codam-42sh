/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   input_parse_ctrl_u.c                               :+:    :+:            */
/*                                                     +:+                    */
/*   By: rkuijper <rkuijper@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/08/23 12:15:47 by rkuijper       #+#    #+#                */
/*   Updated: 2019/08/23 15:08:44 by rkuijper      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"
#include <term.h>

int			input_parse_ctrl_u(t_vshdata *data)
{
	char		*tc_clear_lines_str;
	
	if (data->index > 0)
	{
		if (data->line_copy != NULL)
			ft_strdel(&vshdata->line_copy);
		data->line_copy = ft_strndup(data->line, data->index);
		ft_memcpy(data->line, &vshdata->line[data->index], data->index);
		ft_bzero(&vshdata->line[data->index], data->len_cur - data->index);
		curs_go_home(data, data);
		ft_printf("\e[%iD", data->prompt_len);
		tc_clear_lines_str = tgetstr("cd", NULL);
		if (tc_clear_lines_str == NULL)
		{
			ft_eprintf("ERROR\n"); // DEBUG PRINT
			return (FUNCT_ERROR); // do fatal shit
		}
		tputs(tc_clear_lines_str, 20, &ft_tputchar);
		shell_display_prompt(data, data->cur_prompt_type);		
		data->len_cur = ft_strlen(data->line);
		data->index = data->len_cur;
		input_print_str(data, data->line);
		curs_go_home(data, data);
	}
	return (FUNCT_SUCCESS);
}
