/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   input_parse_ctrl_k.c                               :+:    :+:            */
/*                                                     +:+                    */
/*   By: rkuijper <rkuijper@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/05/16 13:48:04 by rkuijper       #+#    #+#                */
/*   Updated: 2019/08/29 12:03:37 by jbrinksm      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"
#include <term.h>

void		input_parse_ctrl_k(t_vshdata *data)
{
	if (data->line->index < data->line->len_cur)
	{
		if (data->line->line_copy != NULL)
			ft_strdel(&data->line->line_copy);
		data->line->line_copy = ft_strdup(&data->line->line[data->line->index]);
		ft_bzero(&data->line->line[data->line->index],
			data->line->len_cur - data->line->index);
		curs_go_home(data);
		tputs(data->termcaps->tc_clear_lines_str, 20, &ft_tputchar); // why 20?
		data->line->len_cur = ft_strlen(data->line->line);
		data->line->index = data->line->len_cur;
		input_print_str(data, data->line->line);
	}
}
