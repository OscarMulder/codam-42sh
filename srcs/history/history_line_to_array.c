/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   history_line_to_array.c                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: mavan-he <mavan-he@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/05/30 18:55:25 by mavan-he       #+#    #+#                */
/*   Updated: 2019/07/09 16:48:48 by omulder       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"
#include "libft.h"
#include "vsh_history.h"

int		history_line_to_array(char **line)
{
	int	i;

	i = 0;
	while (i < HISTORY_MAX - 1 && history_copy[i])
	{
		if (i != history_tmp && ft_strequ(history[i], history_copy[i]) == 0)
		{
			ft_strdel(&history[i]);
			history[i] = ft_strdup(history_copy[i]); // malloc protec
		}
		i++;
	}
	if (history[history_cur])
		ft_strdel(&history[history_cur]);
	history[history_cur] = ft_strdup(*line); // malloc protec
	*line = ft_strdup(history_copy[history_tmp]); // malloc protec
	if (history_i >= HISTORY_MAX)
	{
		i = HISTORY_MAX - 1;
		while (i > 0)
		{
			history[i] = history[i - 1];
			i++;
		}
	}
	history_i++;
	history_tmp = (history_i < HISTORY_MAX) ? history_i : HISTORY_MAX - 1;
	history_cur = history_tmp;
	return (FUNCT_SUCCESS);
}
