/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   input_parse_ctrl_u.c                               :+:    :+:            */
/*                                                     +:+                    */
/*   By: rkuijper <rkuijper@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/08/23 12:15:47 by rkuijper       #+#    #+#                */
/*   Updated: 2019/08/23 14:13:46 by rkuijper      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"
#include <term.h>

int			input_parse_ctrl_u(t_inputdata *data, t_vshdata *vshdata)
{
	char		*tc_clear_lines_str;
	
	if (data->index > 0)
	{
		if (vshdata->line_copy != NULL)
			ft_strdel(&vshdata->line_copy);
		vshdata->line_copy = ft_strndup(vshdata->line, data->index);
		ft_memcpy(vshdata->line, &vshdata->line[data->index], data->index);
		ft_bzero(&vshdata->line[data->index], data->len_cur - data->index);
		ft_eprintf("New line: %s\n", vshdata->line);
		curs_go_home(data);
		ft_printf("\e[%iD", vshdata->prompt_len);
		tc_clear_lines_str = tgetstr("cd", NULL);
		if (tc_clear_lines_str == NULL)
		{
			ft_eprintf("ERROR\n"); // DEBUG PRINT
			return (FUNCT_ERROR); // do fatal shit
		}
		tputs(tc_clear_lines_str, 20, &ft_tputchar);
		shell_display_prompt(vshdata, vshdata->cur_prompt_type);		
		data->len_cur = ft_strlen(vshdata->line);
		data->index = data->len_cur;
		input_print_str(data, vshdata->line);
		curs_go_home(data);
	}
	return (FUNCT_SUCCESS);
}
