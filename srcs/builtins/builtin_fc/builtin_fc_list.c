/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   builtin_fc_list.c                                  :+:    :+:            */
/*                                                     +:+                    */
/*   By: omulder <omulder@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/09/11 12:54:36 by omulder        #+#    #+#                */
/*   Updated: 2019/09/23 16:28:45 by omulder       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"

/*
** These 3 functions are also used in fc_edit(), so think about that before
** changing them. Maybe I should move them to another file.. <- omulder
*/

void	fc_find_start_end_no_param(t_datahistory *history, int *start, int *end)
{
	*end = history->hist_start;
	*start = *end - 15;
	if (*start < 0 && history->history[0]->number != 1)
		*start = HISTORY_MAX + *start;
	else if (*start < 0)
		*start = 0;
}

int		fc_find_start_end(t_datahistory *history, t_fcdata *fc, int *start,
int *end)
{
	if (fc_find_index(history, fc, fc->first, start) == FUNCT_FAILURE)
		return (FUNCT_FAILURE);
	if (fc->last == NULL)
		*end = history->hist_start;
	else
	{
		if (fc_find_index(history, fc, fc->last, end) == FUNCT_FAILURE)
			return (FUNCT_FAILURE);
	}
	return (FUNCT_SUCCESS);
}

int		fc_get_indexes(t_datahistory *history, t_fcdata *fc, int *start,
int *end)
{
	if (fc->first == NULL)
		fc_find_start_end_no_param(history, start, end);
	else
	{
		if (find_start_end(history, fc, &start, &end) == FUNCT_FAILURE)
			return ;
	}
	return (FUNCT_SUCCESS);
}

void	fc_print(t_datahistory *history, t_fcdata *fc, int start,
int end)
{
	if (fc->options & FC_OPT_R && fc->last == NULL)
		fc_print_reverse(start, end, history->history, fc);
	else if (history->history[start]->number > history->history[end]->number
	&& fc->last != NULL)
		fc_print_reverse(end, start, history->history, fc);
	else
		fc_print_regular(start, end, history->history, fc);
}

void	fc_list(t_datahistory *history, t_fcdata *fc)
{
	int start;
	int end;

	if (fc_get_indexes(history, fc, &start, &end) == FUNCT_FAILURE)
		return ;
	fc_print(history, fc, start, end);
	return ;
}
