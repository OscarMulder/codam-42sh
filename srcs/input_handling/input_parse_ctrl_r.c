/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   input_parse_ctrl_r.c                               :+:    :+:            */
/*                                                     +:+                    */
/*   By: omulder <omulder@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/10/23 15:01:03 by omulder        #+#    #+#                */
/*   Updated: 2019/10/23 19:28:33 by omulder       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"
#include <term.h>

static void	clear_line(t_vshdata *data)
{
	curs_move_n_left(data, data->line->index);
	data->line->index = data->prompt->prompt_len;
	curs_move_n_left(data, data->prompt->prompt_len);
	tputs(data->termcaps->tc_clear_lines_str, 1, &ft_tputchar);
}

static void	init(t_vshdata *data)
{
	clear_line(data);
	ft_printf(HIST_SRCH_FIRST "" HIST_SRCH_LAST);
	data->curs->coords.x = ft_strlen(HIST_SRCH_FIRST "" HIST_SRCH_LAST) + 1;
	data->input->searchhistory.active = true;
}

void		input_parse_ctrl_r(t_vshdata *data)
{
	if (data->input->searchhistory.active)
	{
		// go to next history item
	}
	else
		init(data);
}


// static void reset(t_searchhistory *searchhistory)
// {
// 	ft_bzero(searchhistory, sizeof(t_searchhistory));
// }