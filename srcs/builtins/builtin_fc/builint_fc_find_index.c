/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   builint_fc_find_index.c                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: omulder <omulder@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/09/12 18:16:01 by omulder        #+#    #+#                */
/*   Updated: 2019/09/19 19:13:30 by omulder       ########   odam.nl         */
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

static void	find_history_item_num(t_datahistory *history, int num, int *index)
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

static void	find_hist_num_neg(t_datahistory *history, int num, int *index)
{
	int hist_total;

	hist_total = history->history[history->hist_start]->number;
	if (num == 0 || num == -1)
	{
		*index = history->hist_start;
		return ;
	}
	if (num < 0)
		num = hist_total + 1 + num;
	if (num <= 0)
	{
		if (hist_total < HISTORY_MAX)
			*index = 0;
		else
		{
			if (history->hist_start == (HISTORY_MAX - 1))
				*index = 0;
			else if (hist_total > history->history[history->hist_start + 1]->number)
				*index = history->hist_start + 1;
			else
				*index = history->hist_start + 2;
		}
		return ;
	}
	*index = (num - 1) % HISTORY_MAX;
}

int			fc_find_index(t_datahistory *history, t_fcdata *fc,
char *str, int *index)
{
	int	num;

	num = 0;
	if (ft_isdigit(str[0]) || str[0] == '+' || str[0] == '-')
	{
		num = ft_atoi(str);
		if (str == fc->first && num <= 0)
			fc->options |= FC_FIRST_NEG;
		else if (str == fc->last && num <= 0)
			fc->options |= FC_LAST_NEG;
		if (num <= 0)
			find_hist_num_neg(history, num, index);
		else
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
