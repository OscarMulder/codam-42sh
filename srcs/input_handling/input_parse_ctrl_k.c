/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   input_parse_ctrl_k.c                               :+:    :+:            */
/*                                                     +:+                    */
/*   By: rkuijper <rkuijper@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/05/16 13:48:04 by rkuijper       #+#    #+#                */
/*   Updated: 2019/08/23 15:08:32 by rkuijper      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"
#include <term.h>

int			input_parse_ctrl_k(t_vshdata *data)
{
	char		*tc_clear_lines_str;
	
	if (data->index < data->len_cur)
	{
		if (data->line_copy != NULL)
			ft_strdel(&vshdata->line_copy);
		data->line_copy = ft_strdup(&vshdata->line[data->index]);
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
	}
	return (FUNCT_SUCCESS);
}
