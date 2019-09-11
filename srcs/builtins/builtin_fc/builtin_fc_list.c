/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   builtin_fc_list.c                                  :+:    :+:            */
/*                                                     +:+                    */
/*   By: omulder <omulder@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/09/11 12:54:36 by omulder        #+#    #+#                */
/*   Updated: 2019/09/11 13:03:57 by omulder       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"

static void	find_start(t_history **history, int *start)
{
	int		i;
	int		smallest;

	i = 0;
	smallest = HISTORY_MAX + 1;
	while (i < HISTORY_MAX && history[i]->str != NULL)
	{
		if (history[i]->number < smallest)
		{
			*start = i;
			smallest = history[i]->number;
		}
		i++;
	}
}

void		history_print(t_history **history)
{
	int		i;
	int		start;

	find_start(history, &start);
	i = start;
	while (i < HISTORY_MAX && history[i]->str != NULL)
	{
		print_history_line(history[i]);
		i++;
	}
	i = 0;
	while (start != 0 && i < start && history[i]->str != NULL)
	{
		print_history_line(history[i]);
		i++;
	}
}
