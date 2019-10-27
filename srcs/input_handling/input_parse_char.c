/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   input_parse_char.c                                 :+:    :+:            */
/*                                                     +:+                    */
/*   By: omulder <omulder@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/05/16 13:33:54 by rkuijper       #+#    #+#                */
/*   Updated: 2019/10/27 21:11:04 by omulder       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"
#include "term.h"

void		ctrlr_clear_line(t_vshdata *data)
{
	data->line->index = data->input->searchhistory.total_len;
	data->prompt->prompt_len = ft_strlen(HIST_SRCH_FIRST);
	curs_move_n_left(data, data->input->searchhistory.total_len
	- data->prompt->prompt_len);
	curs_move_n_left(data, ft_strlen(HIST_SRCH_FIRST));
	tputs(data->termcaps->tc_clear_lines_str, 1, &ft_tputchar);
}

int			input_parse_char(t_vshdata *data)
{
	if (data->input->searchhistory.active)
	{
		data->line->index = ft_strlen(data->line->line);
		if (input_parse_char_og(data) == FUNCT_ERROR)
			return (FUNCT_ERROR);
		ctrlr_clear_line(data);
		if (data->input->searchhistory.active == false)
			return (FUNCT_SUCCESS);
		if (history_ctrl_r(data, false) == FUNCT_ERROR)
			return (FUNCT_ERROR);
		input_print_ctrl_r(data, data->line->line,
		data->input->searchhistory.result_str,
		data->input->searchhistory.result_i);
	}
	else
		input_parse_char_og(data);
	return (FUNCT_SUCCESS);
}
