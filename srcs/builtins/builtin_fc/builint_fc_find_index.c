/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   builint_fc_find_index.c                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: omulder <omulder@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/09/12 18:16:01 by omulder        #+#    #+#                */
/*   Updated: 2019/10/15 18:33:04 by omulder       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"

static int	find_history_item_str(t_datahistory *history, char *match,
t_historyitem **item)
{
	size_t	len;

	len = ft_strlen(match);
	*item = history->tail;
	while ((*item) != NULL)
	{
		if (ft_strnequ((*item)->str, match, len) == true)
			return (FUNCT_SUCCESS);
		if ((*item)->prev == NULL)
			return (FUNCT_FAILURE);
		*item = (*item)->prev;
	}
	return (FUNCT_FAILURE);
}

static void	find_history_item_num(t_datahistory *history, int num,
t_historyitem **item)
{
	*item = history->head;
	while ((*item)->number != num)
	{
		if ((*item)->next == NULL)
			break ;
		*item = (*item)->next;
	}
}

static void	find_hist_num_neg(t_datahistory *history, int num,
t_historyitem **item)
{
	int i;

	i = 0;
	*item = history->tail;
	while (i > num)
	{
		if ((*item)->prev == NULL)
			break ;
		*item = (*item)->prev;
		i--;
	}
}

int			fc_find_item(t_datahistory *history, t_fcdata *fc,
char *str, t_historyitem **item)
{
	int	num;

	num = 0;
	if (ft_isdigit(str[0]) || ((str[0] == '+' || str[0] == '-') &&
	ft_isdigit(str[1])))
	{
		num = ft_atoi(str);
		if (str == fc->first && num <= 0)
			fc->options |= FC_FIRST_NEG;
		else if (str == fc->last && num <= 0)
			fc->options |= FC_LAST_NEG;
		if (num <= 0)
			find_hist_num_neg(history, num, item);
		else
			find_history_item_num(history, num, item);
		if (fc->options & FC_OPT_L && item != NULL && (*item)->next == NULL && (*item)->prev != NULL)
			*item = (*item)->prev;
	}
	else
	{
		if (find_history_item_str(history, str, item) == FUNCT_FAILURE)
		{
			err_void_exit(E_FC_OUT_RANGE, EXIT_FAILURE);
			return (FUNCT_FAILURE);
		}
	}
	return (FUNCT_SUCCESS);
}
