/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   builint_fc_find_index.c                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: omulder <omulder@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/09/12 18:16:01 by omulder        #+#    #+#                */
/*   Updated: 2019/09/16 16:30:22 by omulder       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"

static int	find_history_item_str(t_datahistory *history, char *match,
int *index)
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
	if (i <= 0)
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

static void	find_hist_num_backup(t_datahistory *history, int num, int *index)
{
	int i;

	i = history->hist_start;
	while (i >= 0 && history->history[i]->str != NULL)
	{
		if (history->history[i]->number == num)
		{
			*index = i;
			return ;
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
			return ;
		}
		i--;
	}
	*index = history->hist_start;
}

static int	set_max_num(t_datahistory *history)
{
	int	his_max_num;

	if (history->history[0]->number == 1)
		his_max_num = history->history[history->hist_start]->number;
	else if ((history->hist_start + 1) == HISTORY_MAX)
		his_max_num = history->history[0]->number;
	else
		his_max_num = history->history[history->hist_start + 1]->number;
	return (his_max_num);
}

static void	find_history_item_num(t_datahistory *history, int num, int *index)
{
	int	his_max_num;

	his_max_num = set_max_num(history);
	if (num < 0 && (num * -1) > HISTORY_MAX)
		num = (HISTORY_MAX * -1);
	if (num < 0)
		num = his_max_num + 1 + num;
	if (num == 0 || num > his_max_num)
	{
		*index = history->hist_start;
		return ;
	}
	if (num <= (his_max_num - HISTORY_MAX))
	{
		if (history->history[history->hist_start]->number > (HISTORY_MAX - 1))
			*index = history->hist_start + 1;
		else
			*index = 0;
		return ;
	}
	if (history->history[(num - 1) % HISTORY_MAX]->str != NULL &&
	history->history[(num - 1) % HISTORY_MAX]->number == num)
		*index = (num - 1) % HISTORY_MAX;
	else
		find_hist_num_backup(history, num, index);
}

int			fc_find_index(t_datahistory *history, t_fcdata *fc,
char *str, int *index)
{
	int	num;

	num = 0;
	if (ft_isdigit(str[0]) || str[0] == '+' || str[0] == '-')
	{
		num = ft_atoi(str);
		if (num < 0 && fc->options & ~FC_FIRST_NEG)
			fc->options |= FC_FIRST_NEG;
		else if (num < 0)
			fc->options |= FC_LAST_NEG;
		find_history_item_num(history, num, index);
	}
	else
	{
		if (find_history_item_str(history, str, index) == FUNCT_FAILURE)
		{
			ft_eprintf(E_FC_OUT_RANGE);
			return (FUNCT_FAILURE);
		}
	}
	return (FUNCT_SUCCESS);
}
