/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   builtin_fc_list.c                                  :+:    :+:            */
/*                                                     +:+                    */
/*   By: omulder <omulder@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/09/11 12:54:36 by omulder        #+#    #+#                */
/*   Updated: 2019/09/12 15:13:42 by omulder       ########   odam.nl         */
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
		fc_list_print_line(history[(start + i)], fc);
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

int			find_history_item_str(t_datahistory *history, char *match, int *index)
{
	int i;

	i = history->hist_start;
	while (i >= 0 && history->history[i]->str != NULL)
	{
		if (ft_strncmp(history->history[i]->str, match, ft_strlen(match)) == 0)
		{
			*index = i;
			return (FUNCT_SUCCESS);
		}
		i--;
	}
	if (i == 0)
		i = (HISTORY_MAX - 1);
	while (i > history->hist_start && history->history[i]->str != NULL)
	{
		if (ft_strncmp(history->history[i]->str, match, ft_strlen(match)) == 0)
		{
			*index = i;
			return (FUNCT_SUCCESS);
		}
		i--;
	}
	return (FUNCT_FAILURE);
}

int			find_hist_num_backup(t_datahistory *history, int num, int *index)
{
	int i;

	i = history->hist_start;
	while (i >= 0 && history->history[i]->str != NULL)
	{
		if (history->history[i]->number == num)
		{
			*index = i;
			return (FUNCT_SUCCESS);
		}
		i--;
	}
	if (i == 0)
		i = (HISTORY_MAX - 1);
	while (i >= history->hist_start && history->history[i]->str != NULL)
	{
		if (history->history[i]->number == num)
		{
			*index = i;
			return (FUNCT_SUCCESS);
		}
		i--;
	}
	return (FUNCT_FAILURE);
}

int			find_history_item_num(t_datahistory *history, int num, int *index)
{
	int	his_max_num;

	his_max_num = history->history[history->hist_start]->number;
	if (num < 0)
		num = his_max_num + 1 + num;
	if (num == 0)
		return (FUNCT_FAILURE);
	if (num > his_max_num || num <= his_max_num - HISTORY_MAX)
		return (FUNCT_FAILURE);
	else if (history->history[(num - 1) % HISTORY_MAX]->str != NULL &&
	history->history[(num - 1) % HISTORY_MAX]->number == num)
	{
		*index = (num - 1) % HISTORY_MAX;
		return (FUNCT_SUCCESS);
	}
	return (find_hist_num_backup(history, num, index));
}

int			find_index(t_datahistory *history, char *str, int *index)
{
	if (ft_isdigit(str[0]) || str[0] == '+' || str[0] == '-')
	{
		if (find_history_item_num(history, ft_atoi(str), index) == FUNCT_FAILURE)
		{
			ft_eprintf("ERROR4\n");
			return (FUNCT_FAILURE);
		}
	}
	else
	{
		if (find_history_item_str(history, str, index) == FUNCT_FAILURE)
		{
			ft_eprintf("ERROR3\n");
			return (FUNCT_FAILURE);
		}
	}
	return (FUNCT_SUCCESS);
}

void		fc_list(t_datahistory *history, t_fcdata *fc)
{
	int start;
	int end;

	if (fc->first == NULL)
	{
		end = history->hist_start;
		start = end - 15;
		if (start < 0 && history->history[0]->number != 1)
			start = HISTORY_MAX - start;
		else if (start < 0)
			start = 0;
	}
	else
	{
		if (find_index(history, fc->first, &start) == FUNCT_ERROR)
		{
			ft_eprintf("ERROR1\n");
			return ;
		}
		if (fc->last == NULL)
			end = history->hist_start;
		else
		{
			if (find_index(history, fc->last, &end) == FUNCT_ERROR)
			{
				ft_eprintf("ERROR2\n");
				return ;
			}
		}
	}
	if (fc->options & FC_OPT_R)
		print_reverse(start, end, history->history, fc);
	else
		print_regular(start, end, history->history, fc);
}
