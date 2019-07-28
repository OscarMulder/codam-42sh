/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   history_line_to_array.c                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: mavan-he <mavan-he@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/05/30 18:55:25 by mavan-he       #+#    #+#                */
/*   Updated: 2019/07/28 15:48:29 by omulder       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"
#include "libft.h"

/*
** Add one line to the history array
*/

static void	history_find_start(t_history **history, int *number, int *start)
{
	int i;
	int	smallest;

	i = 0;
	smallest = HISTORY_MAX + 1;
	while (i < HISTORY_MAX && history[i]->str != NULL)
		i++;
	if (i != HISTORY_MAX)
	{
		*start = i;
		*number = i;
		return ;
	}
	while (i < HISTORY_MAX && history[i] != NULL)
	{
		if (history[i]->number < smallest)
		{
			*start = i + 1;
			smallest = history[i]->number;
		}
		if (history[i]->number > *number)
			*number = history[i]->number;
		i++;
	}
}

int			history_line_to_array(t_history **history, char *line)
{
	int start;
	int number;
	int i;

	if (ft_strlen(line) <= 1)
		return (true);
	number = -1;
	start = 0;
	history_find_start(history, &number, &start);
	i = start;
	if (history[i]->str != NULL)
		ft_strdel(&history[i]->str);
	history[i]->number = number + 1;
	history[i]->str = ft_strsub(line, 0, ft_strlen(line) - 1);
	if (history[i]->str == NULL)
		return (false);
	return (true);
}
