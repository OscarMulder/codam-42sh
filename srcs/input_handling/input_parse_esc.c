/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   input_parse_esc.c                                  :+:    :+:            */
/*                                                     +:+                    */
/*   By: omulder <omulder@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/10/23 16:48:32 by omulder        #+#    #+#                */
/*   Updated: 2019/10/27 19:20:46 by omulder       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"
#include <term.h>

static void	reset(t_searchhistory *searchhistory)
{
	ft_bzero(searchhistory, sizeof(t_searchhistory));
}

void		input_parse_esc(t_vshdata *data)
{
	char	*tmp;

	if (data->input->searchhistory.active)
	{
		data->input->searchhistory.active = false;
		ctrlr_clear_line(data);
		shell_display_prompt(data, REGULAR_PROMPT);
		tmp = data->line->line;
		data->line->line = data->input->searchhistory.result_str;
		data->line->len_max = ft_strlen(data->input->searchhistory.result_str);
		data->line->len_cur = data->line->len_max;
		data->line->index = data->line->len_max;
		input_print_str(data, data->line->line);
		ft_strdel(&tmp);
		reset(&data->input->searchhistory);
	}
}
