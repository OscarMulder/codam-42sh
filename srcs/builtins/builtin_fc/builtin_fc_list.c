/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   builtin_fc_list.c                                  :+:    :+:            */
/*                                                     +:+                    */
/*   By: omulder <omulder@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/09/11 12:54:36 by omulder        #+#    #+#                */
/*   Updated: 2019/10/15 18:22:37 by omulder       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"

static void	fc_find_start_len_no_param(t_datahistory *history, t_fcdata *fc,
t_historyitem **start, int *len)
{
	int i;

	i = 16;
	if (fc->options & FC_OPT_L)
	{
		*start = history->tail;
		*len = 16;
		while (i > 0)
		{
			if ((*start)->prev == NULL)
				break ;
			*start = (*start)->prev;
			i--;
		}
	}
	else
	{
		*start = history->tail;
		*len = 1;
	}
}

static int	fc_find_start_len(t_datahistory *history, t_fcdata *fc,
t_historyitem **start, int *len)
{
	t_historyitem *end;

	if (fc_find_item(history, fc, fc->first, start) == FUNCT_FAILURE)
		return (FUNCT_FAILURE);
	if (fc->last == NULL && fc->options & FC_OPT_L && fc->options & ~FC_OPT_R)
		*len = history_count(*start, NULL) - 2;
	else if (fc->last == NULL && fc->options & FC_OPT_L)
		*len = history->count;
	else if (fc->last == NULL)
		*len = 1;
	else
	{
		if (fc_find_item(history, fc, fc->last, &end) == FUNCT_FAILURE)
			return (FUNCT_FAILURE);
		*len = history_count(*start, end);
		if (*len == -1)
		{
			*len = history_count(end, *start);
			fc->options |= FC_OPT_R;
		}
	}
	if (*len < 1)
		*len = 1;
	return (FUNCT_SUCCESS);
}

int		fc_get_start(t_datahistory *history, t_fcdata *fc,
t_historyitem **start, int *len)
{
	if (fc->first == NULL)
		fc_find_start_len_no_param(history, fc, start, len);
	else
	{
		if (fc_find_start_len(history, fc, start, len) == FUNCT_FAILURE)
			return (FUNCT_FAILURE);
	}
	return (FUNCT_SUCCESS);
}

void	fc_print(t_fcdata *fc, t_historyitem *start, int len)
{
	t_historyitem *end;

	end = history_walker(start, len);
	// if (start != NULL && end != NULL)
	// 	ft_eprintf("start: %x, end: %x, start->num: %d, end->num: %d, len: %d\n", start, end, start->number, end->number, len);
	if (fc->options & FC_OPT_R)
		fc_print_reverse(start, len, fc);
	else
		fc_print_regular(start, len, fc);
}

void	fc_list(t_datahistory *history, t_fcdata *fc)
{
	t_historyitem	*start;
	int				len;

	if (fc_get_start(history, fc, &start, &len) == FUNCT_FAILURE)
		return ;
	fc_print(fc, start, len);
	return ;
}
