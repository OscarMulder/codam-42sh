/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   input_parse_ctrl_r.c                               :+:    :+:            */
/*                                                     +:+                    */
/*   By: omulder <omulder@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/10/23 15:01:03 by omulder        #+#    #+#                */
/*   Updated: 2019/10/25 14:55:30 by omulder       ########   odam.nl         */
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
	int index_tmp;

	index_tmp = data->line->index;
	data->input->searchhistory.current = data->history->tail;
	clear_line(data);
	input_print_str(data, HIST_SRCH_FIRST "" HIST_SRCH_LAST);
	input_print_str(data, data->line->line);
	// ft_eprintf("i_tmp: %d, index: %d, len-cur: %d, strlen: %d\n", index_tmp, data->line->index, data->line->len_cur, ft_strlen(data->line->line));
	// ft_eprintf("i_tmp: %d, index: %d, len-cur: %d, strlen: %d\n", index_tmp, data->line->index, data->line->len_cur, ft_strlen(data->line->line));
	data->line->index = data->line->len_cur;
	curs_move_n_left(data, data->line->len_cur - index_tmp);
	data->input->searchhistory.total_len = ft_strlen(HIST_SRCH_FIRST "" HIST_SRCH_LAST) + index_tmp;
	data->input->searchhistory.result_str = data->line->line;
	data->input->searchhistory.result_i = ft_strlen(data->line->line) - 1;
	data->line->index = 0;
	data->line->len_max = 64;
	data->line->len_cur = 0;
	data->line->line = ft_strnew(data->line->len_max); // malloc check
	data->input->searchhistory.active = true;
}

void		input_parse_ctrl_r(t_vshdata *data)
{
	if (data->input->searchhistory.active)
	{
		ft_eprintf("I AM IN CTRL R\n");
		ctrlr_clear_line(data);
		data->line->index = data->line->len_cur;
		input_print_str(data, HIST_SRCH_FIRST);
		input_print_str(data, data->line->line);
		input_print_str(data, HIST_SRCH_LAST);
		ft_eprintf("current: %p\n", data->input->searchhistory.current);
		if (data->input->searchhistory.current != NULL)
			data->input->searchhistory.current = data->input->searchhistory.current->prev;
		if (history_ctrl_r(data, true) == FUNCT_ERROR)
			return ;
		input_print_str(data, data->input->searchhistory.result_str);
		ft_eprintf("result: %s\n", data->input->searchhistory.result_str);
		data->line->index = ft_strlen(data->input->searchhistory.result_str);
		curs_move_n_left(data, data->line->index - data->input->searchhistory.result_i);
		data->input->searchhistory.total_len = ft_strlen(HIST_SRCH_FIRST "" HIST_SRCH_LAST) + data->line->len_cur + data->input->searchhistory.result_i;
		data->line->index = data->line->len_cur;
	}
	else
		init(data);
}
