/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   builtin_fc_list.c                                  :+:    :+:            */
/*                                                     +:+                    */
/*   By: omulder <omulder@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/09/11 12:54:36 by omulder        #+#    #+#                */
/*   Updated: 2019/09/11 16:56:19 by omulder       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"

/* static void	find_start(t_history **history, int *start)
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
} */

/*
** Start and end have to be valid indexes on the history_array
*/

static void	print_regular(int start, int end, t_history **history, t_fcdata *fc)
{
	int i;

	i = 0;
	while ((start + i) < HISTORY_MAX &&
	((end >= start && (start + i) <= end) || end < start) &&
	history[start + i]->str != NULL)
	{
		fc_list_print_line(history[start + i], fc);
		i++;
	}
	if ((start + i) > HISTORY_MAX && end < start)
	{
		i = 0;
		while (i < end && history[i]->str != NULL)
		{
			fc_list_print_line(history[i], fc);
			i++;
		}
	}
}

/*
** Start and end have to be valid indexes on the history_array.
** The function will start printing at end and will decrement untill it reaches
** start. This is counterintuitive but hey it says reverse so....
*/

static void	print_reverse(int start, int end, t_history **history, t_fcdata *fc)
{
	int i;

	i = 0;
	while ((end - i) >= 0 &&
	((end >= start && (end - i) >= start) || start > end) &&
	history[end - i]->str != NULL)
	{
		fc_list_print_line(history[end - i], fc);
		i++;
	}
	if ((end - i) == 0 && start > end)
	{
		i = HISTORY_MAX - 1;
		while (i > start && history[i]->str != NULL)
		{
			fc_list_print_line(history[i], fc);
			i--;
		}
	}
}

/* static void	find_end(int start, int *end)
{
	if (start == 0)
		*end = HISTORY_MAX - 1;
	else
		*end = start - 1;
} */

void		fc_list(t_datahistory *history, t_fcdata *fc)
{
	int start;
	int end;

	if (fc->first == NULL)
	{
		end = history->hist_start + 1;
		start = end - 16;
		ft_eprintf("start: %d\nend: %d\n", start, end);
		if (start < 0 && history->history[0]->number != 1)
			start = HISTORY_MAX - start;
		else
			start = 0;
	}
	if (fc->options & FC_OPT_R)
		print_reverse(start, end, history->history, fc);
	else
		print_regular(start, end, history->history, fc);
}
