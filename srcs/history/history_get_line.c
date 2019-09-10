/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   history_get_line.c                                 :+:    :+:            */
/*                                                     +:+                    */
/*   By: mavan-he <mavan-he@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/09/10 18:35:45 by mavan-he       #+#    #+#                */
/*   Updated: 2019/09/10 18:42:18 by mavan-he      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"

static char	*history_get_line_at_index(t_datahistory *history, int i)
{
	(void)history;
	(void)i;
	return (NULL);
}


char		*history_get_line(t_datahistory *history, char *line, size_t i)
{
	if (ft_isdigit(line[i + 1]) || (line[i + 1] == '-' && 
		ft_isdigit(line[i + 2])))
		return (history_get_line_at_index(history, ft_atoi(&line[i + 1])));
	else if (line[i + 1] == '!')
		return (history_get_line_at_index(history, -1));
	
	ft_printf("find match line");
	return (NULL);
//		return (history_match_line(history, line, i)); // write func
}