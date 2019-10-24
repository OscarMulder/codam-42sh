/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   history_ctrl_r.c                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: omulder <omulder@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/10/24 16:54:12 by omulder        #+#    #+#                */
/*   Updated: 2019/10/24 20:08:20 by omulder       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"

static int	set_new_match(t_vshdata *data, char *match, char *full_str)
{
	data->input->searchhistory.result_i = match - full_str;
	ft_strdel(&data->input->searchhistory.result_str);
	data->input->searchhistory.result_str = ft_strdup(full_str);
	if (data->input->searchhistory.result_str == NULL)
		return (FUNCT_ERROR); // do error stuffs
	return (FUNCT_SUCCESS);
}

static int	history_find_match(t_vshdata *data, char *to_match)
{
	t_historyitem	*probe;
	char			*match;

	probe = data->input->searchhistory.current;
	while (probe != NULL)
	{
		match = ft_strstr(probe->str, to_match);
		if (match != NULL)
			return (set_new_match(data, match, probe->str));
		probe = probe->prev;
	}
	return (FUNCT_FAILURE);
}

int			history_ctrl_r(t_vshdata *data)
{
	char	*match;

	match = ft_strstr(data->input->searchhistory.result_str, data->line->line);
	if (match == NULL)
		return (history_find_match(data, data->line->line));
	else
		return (set_new_match(data, match, data->input->searchhistory.result_str));
	return (history_find_match(data, data->line->line));
}
