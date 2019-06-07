/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   history_line_to_array.c                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: mavan-he <mavan-he@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/05/30 18:55:25 by mavan-he       #+#    #+#                */
/*   Updated: 2019/06/03 16:59:17 by mavan-he      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"
#include "vsh_history.h"
#include "libft.h"

int		history_line_to_array(char **line)
{
	int	i;

	/*
	** Loop through the history and check if there are changed strings
	** other than the last modified string (history_tmp).
	** If there are, overwrite the original history with the history copy.
	*/
	i = 0;
	while (i < HISTORY_MAX && history_copy[i])
	{
		if (i != history_tmp && ft_strequ(history[i], history_copy[i]) == 0)
		{
			ft_strdel(&history[i]);
			history[i] = ft_strdup(history_copy[i]);
			// Failsave check.
		}
		else if (i == history_tmp)
		{
			ft_printf("\nCopying over current string from %d to %d: %s\n", history_tmp, history_cur, history_copy[history_tmp]);
			if (history[history_cur])
				ft_strdel(&history[history_cur]);
			history[history_cur] = ft_strdup(history_copy[i]);
			ft_printf("Copy: %s\n", history[history_cur]);
		}
		i++;
	}
	// Failsave check
	*line = ft_strdup(history_copy[history_tmp]);
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
