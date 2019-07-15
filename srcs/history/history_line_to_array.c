/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   history_line_to_array.c                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: mavan-he <mavan-he@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/05/30 18:55:25 by mavan-he       #+#    #+#                */
/*   Updated: 2019/07/15 16:56:39 by omulder       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"
#include "vsh_history.h"
#include "libft.h"

/*
** Add one line to the history array
*/

static void	find_start(t_history **history, int *number, int *start)
{
	int i;
	int	smallest;

	i = 0;
	*start = 0;
	*number = -1;
	smallest = HISTORY_MAX + 1;
	while (i < HISTORY_MAX && history[i] != NULL)
	{
		if (history[i]->number < smallest)
		{
			*start = i;
			smallest = history[i]->number;
		}
		if (history[i]->number > *number)
			*number = history[i]->number;
		i++;
	}
}

int		history_line_to_array(t_history **history, char *line)
{
	int start;
	int number;
	int i;

	find_start(history, &number, &start);
	i = start;
	if (history[i] != NULL)
		ft_strdel(&history[i]->str);
	history[i]->str = ft_strdup(line);
	if (history[i]->str == NULL)
		return (false);
	history[i]->number = number + 1;
	return (true);
}
