/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   auto_add_match_toline.c                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: mavan-he <mavan-he@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/08/12 20:55:01 by mavan-he       #+#    #+#                */
/*   Updated: 2019/08/16 04:37:03 by mavan-he      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"

int		auto_add_match_toline(char *match, char *to_add, char **line,
t_inputdata *data)
{
	int		line_len;
	int		match_len;
	int		to_add_len;
	char	*new_line;

	line_len = ft_strlen(*line);
	match_len = ft_strlen(match);
	to_add_len = ft_strlen(to_add);
	new_line = ft_strnew(line_len - match_len + to_add_len);
	data->len_max = line_len - match_len + to_add_len;
	if (new_line == NULL)
	{
		ft_eprintf(E_ALLOC_STR);
		return (FUNCT_ERROR);
	}
	ft_strncpy(new_line, *line, data->index - match_len);
	ft_strcat(new_line, to_add);
	ft_strcat(new_line, &(*line)[data->index]);
	#ifdef DEBUG
	ft_eprintf("----------\nOld line: %s\nNew line: %s\n", *line, new_line); // debugging
	ft_eprintf("Orignal index: %d :", data->index); // debugging
	#endif
	ft_strdel(line);
	*line = new_line;
	data->index = data->index - match_len + to_add_len;
	data->len_cur = ft_strlen(*line);
	#ifdef DEBUG
	ft_eprintf(" New index: %d\n ---------\n", data->index); // debugging
	#endif
	ft_printf("%s", &(to_add[match_len]));
	return (AUTO_STATE_LINE);
}
