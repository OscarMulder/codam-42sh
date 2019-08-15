/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   auto_add_match_toline.c                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: mavan-he <mavan-he@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/08/12 20:55:01 by mavan-he       #+#    #+#                */
/*   Updated: 2019/08/15 10:49:55 by omulder       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"

int		auto_add_match_toline(char *match, char *to_add, char **line, int *i)
{
	int		line_len;
	int		match_len;
	int		to_add_len;
	char	*new_line;

	line_len = ft_strlen(*line);
	match_len = ft_strlen(match);
	to_add_len = ft_strlen(to_add);
	new_line = ft_strnew(line_len - match_len + to_add_len);
	if (new_line == NULL)
	{
		ft_eprintf(E_ALLOC_STR);
		return (FUNCT_ERROR);
	}
	ft_strncpy(new_line, *line, *i - match_len);
	ft_strcat(new_line, to_add);
	ft_strcat(new_line, &(*line)[*i]);
	ft_printf("----------\nOld line: %s\nNew line: %s\n", *line, new_line); // debugging
	*line = new_line;
	ft_printf("Orignal index: %d :", *i); // debugging
	*i = *i - match_len + to_add_len;
	ft_printf(" New index: %d\n ---------\n", *i); // debugging
	return (FUNCT_SUCCESS);
}
