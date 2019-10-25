/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   input_parse_backspace.c                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: rkuijper <rkuijper@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/05/16 13:43:07 by rkuijper       #+#    #+#                */
/*   Updated: 2019/10/25 13:47:53 by omulder       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"
#include <term.h>

/*
**	Backspaces are handled saving the cursor position and then clearing the
**	screen and then reprinting the edited line and then going back to the old
**	cursor position.
*/

static void	input_handle_backspace_og(t_vshdata *data)
{
	int			len_left;
	int			saved_index;

	if (data->line->index > 0)
	{
		saved_index = data->line->index;
		len_left = data->line->len_cur - data->line->index;
		data->line->len_cur--;
		curs_go_home(data);
		input_clear_char_at(&data->line->line, saved_index - 1);
		tputs(data->termcaps->tc_clear_lines_str, 1, &ft_tputchar);
		input_print_str(data, data->line->line);
		data->line->index = data->line->len_cur;
		curs_move_n_left(data, len_left);
	}
}

void	input_handle_backspace(t_vshdata *data)
{
	int			len_left;
	int			saved_index;

	if (data->input->searchhistory.active && data->line->index > 0)
	{
		ctrlr_clear_line(data);
		saved_index = ft_strlen(data->line->line);
		len_left = data->line->len_cur - data->line->index;
		data->line->len_cur--;
		input_clear_char_at(&data->line->line, saved_index - 1);
		input_print_str(data, HIST_SRCH_FIRST);
		input_print_str(data, data->line->line);
		input_print_str(data, HIST_SRCH_LAST);
		data->input->searchhistory.current = data->history->tail;
		if (history_ctrl_r(data, true) == FUNCT_ERROR)
			return ;
		input_print_str(data, data->input->searchhistory.result_str);
		data->line->index = ft_strlen(data->input->searchhistory.result_str);
		curs_move_n_left(data, data->line->index - data->input->searchhistory.result_i);
		data->line->index = data->line->len_cur;
		data->input->searchhistory.total_len = ft_strlen(HIST_SRCH_FIRST "" HIST_SRCH_LAST) + data->line->len_cur + data->input->searchhistory.result_i;
		ft_eprintf("PARSE BACKSPACE total: %d\n", data->input->searchhistory.total_len);
	}
	else
		input_handle_backspace_og(data);
}
