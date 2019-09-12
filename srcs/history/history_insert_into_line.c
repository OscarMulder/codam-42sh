/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   history_insert_into_line.c                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: mavan-he <mavan-he@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/09/12 15:06:45 by mavan-he       #+#    #+#                */
/*   Updated: 2019/09/12 15:55:09 by mavan-he      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"

static size_t	history_get_match_len(char *line, size_t i)
{
	size_t len;

	len = 0;
	i++;
	if (ft_isdigit(line[i]) || (line[i] == '-' && ft_isdigit(line[i + 1])))
	{
		if (line[i] == '-')
			len++;
		while (ft_isdigit(line[i + len]) == true)
			len++;
		return (len);
	}
	while (line[i] != '\0' && ft_isblank(line[i]) == false && line[i] != '\n')
	{
		len++;
		i++;
	}
	return (len);
}

int				history_insert_into_line(char **line, char *hist_line, size_t i)
{
	size_t	match_len;
	size_t	hist_len;
	char	*new_line;

	ft_printf("\nLINE = %s \nLINE INDEX: %d\n", *line, i);
	match_len = history_get_match_len(*line, i);
	ft_printf("Matchlen: %d\n", match_len);
	hist_len = ft_strlen(hist_line);
	new_line = ft_memalloc(ft_strlen(*line) - (match_len + 1) + hist_len);
	if (new_line == NULL)
		return (err_ret(E_ALLOC_STR));
	ft_strncpy(new_line, *line, i);
	ft_strcat(new_line, hist_line);
	ft_strcat(new_line, &(*line)[i + match_len + 1]);
	ft_strdel(line);
	*line = new_line;
	return (FUNCT_SUCCESS);
}
