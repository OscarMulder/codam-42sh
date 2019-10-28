/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   input_parse_char.c                                 :+:    :+:            */
/*                                                     +:+                    */
/*   By: omulder <omulder@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/05/16 13:33:54 by rkuijper       #+#    #+#                */
/*   Updated: 2019/10/28 13:59:44 by omulder       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"
#include "term.h"

void		ctrlr_clear_line(t_vshdata *data)
{
	char *tmp;

	tmp = data->line->line;
	data->line->line = ft_strjoin(HIST_SRCH_FIRST, ft_strjoin(tmp, ft_strjoin(HIST_SRCH_LAST, data->input->searchhistory.result_str)));
	data->line->index = data->input->searchhistory.total_len;
	data->prompt->prompt_len = 0;
	curs_move_n_left(data, data->line->index);
	data->line->line = tmp;
	tputs(data->termcaps->tc_clear_lines_str, 1, &ft_tputchar);
}

int			input_parse_char(t_vshdata *data)
{
	if (data->input->searchhistory.active)
	{
		ctrlr_clear_line(data);
		data->line->index = ft_strlen(data->line->line);
		if (input_parse_char_og(data) == FUNCT_ERROR)
			return (FUNCT_ERROR);
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
