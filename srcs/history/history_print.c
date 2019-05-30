/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   history_print.c                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: mavan-he <mavan-he@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/05/30 20:47:41 by mavan-he       #+#    #+#                */
/*   Updated: 2019/05/30 21:11:44 by mavan-he      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"

void	history_print(void)
{
	int i;
	int tmp;

	i = 0;
	tmp = 0;
	if (history_i >= 500)
	{
		tmp = history_i - 500;
		i = history_i % 500;
	}
	while (i < 500 && history[i] != NULL)
	{
		ft_printf("%5d  %s\n", tmp, history[i]);
		i++;
		tmp++;
	}
	if (history_i >= 500)
	{
		i = 0;
		while (i < history_i % 500)
		{
			ft_printf("%5d  %s\n", tmp, history[i]);
			i++;
			tmp++;
		}
	}
}
