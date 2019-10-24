/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   input_parse_esc.c                                  :+:    :+:            */
/*                                                     +:+                    */
/*   By: omulder <omulder@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/10/23 16:48:32 by omulder        #+#    #+#                */
/*   Updated: 2019/10/23 19:30:33 by omulder       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"
#include <term.h>

static void	reset(t_searchhistory *searchhistory)
{
	ft_bzero(searchhistory, sizeof(t_searchhistory));
}

static void	clear_line(t_vshdata *data)
{
	curs_move_n_left(data, data->line->index);
	data->line->index = ft_strlen(HIST_SRCH_FIRST "" HIST_SRCH_LAST);
	if (data->input->searchhistory.search_str != NULL)
		data->line->index += ft_strlen(data->input->searchhistory.search_str);
	curs_move_n_left(data, data->line->index);
	tputs(data->termcaps->tc_clear_lines_str, 1, &ft_tputchar);
}

void		input_parse_esc(t_vshdata *data)
{
	if (data->input->searchhistory.active)
	{
		data->input->searchhistory.active = false;
		clear_line(data);
		reset(&data->input->searchhistory);
		shell_display_prompt(data, REGULAR_PROMPT);
	}
}
