/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   input_parse_esc.c                                  :+:    :+:            */
/*                                                     +:+                    */
/*   By: omulder <omulder@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/10/23 16:48:32 by omulder        #+#    #+#                */
/*   Updated: 2019/10/24 19:59:05 by omulder       ########   odam.nl         */
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
	if (data->input->searchhistory.active)
	{
		data->input->searchhistory.active = false;
		ctrlr_clear_line(data);
		reset(&data->input->searchhistory);
		shell_display_prompt(data, REGULAR_PROMPT);
		input_print_str(data, data->line->line);
		data->line->index = ft_strlen(data->line->line);
		// input_print_str(data, data->line->line);
	}
}
