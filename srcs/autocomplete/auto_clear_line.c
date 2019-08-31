/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   auto_clear_line.c                                  :+:    :+:            */
/*                                                     +:+                    */
/*   By: omulder <omulder@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/08/16 12:11:22 by omulder        #+#    #+#                */
/*   Updated: 2019/08/22 15:41:43 by omulder       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"
#include <term.h>

void	auto_clear_line(t_inputdata *data, t_vshdata *vshdata)
{
	char	*tc_clear_lines_str;

	curs_go_home(data);
	ft_printf("\e[%iD", vshdata->prompt_len);
	tc_clear_lines_str = tgetstr("cd", NULL);
	if (tc_clear_lines_str == NULL)
	{
		ft_eprintf(E_TERM_DB_NOT_F);
		return ;
	}
	tputs(tc_clear_lines_str, 1, &ft_tputchar);
	shell_display_prompt(vshdata);
}
