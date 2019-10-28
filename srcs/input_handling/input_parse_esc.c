/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   input_parse_esc.c                                  :+:    :+:            */
/*                                                     +:+                    */
/*   By: omulder <omulder@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/10/23 16:48:32 by omulder        #+#    #+#                */
/*   Updated: 2019/10/28 15:16:55 by omulder       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"
#include <term.h>

static void	reset(t_searchhistory *searchhistory)
{
	ft_bzero(searchhistory, sizeof(t_searchhistory));
}

void		ctrlr_restore_shell(t_vshdata *data)
{
	char	*tmp;

	data->input->searchhistory.active = false;
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

int			input_parse_esc(t_vshdata *data)
{
	if (data->input->searchhistory.active)
	{
		if (ctrlr_clear_line(data) == FUNCT_ERROR)
			return (FUNCT_ERROR);
		ctrlr_restore_shell(data);
	}
	return (FUNCT_SUCCESS);
}
