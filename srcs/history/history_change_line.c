/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   history_change_line.c                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: mavan-he <mavan-he@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/06/02 14:28:54 by mavan-he       #+#    #+#                */
/*   Updated: 2019/07/15 16:56:53 by omulder       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"
#include "vsh_history.h"
#include "libft.h"

static void	history_clear_line(unsigned *index, unsigned linelen)
{
	if (*index > 0)
		ft_printf("\e[%dD", *index);
	*index = 0;
	while (*index < linelen)
	{
		ft_putchar(' ');
		(*index)++;
	}
	if (*index > 0)
		ft_printf("\e[%dD", *index);
}

// static void	find_start(t_history **history, int *number, int *start)
// {
// 	int i;
// 	int	smallest;

// 	i = 0;
// 	*start = 0;
// 	*number = -1;
// 	smallest = HISTORY_MAX + 1;
// 	while (i < HISTORY_MAX && history[i] != NULL)
// 	{
// 		if (history[i]->number < smallest)
// 		{
// 			*start = i;
// 			smallest = history[i]->number;
// 		}
// 		if (history[i]->number > *number)
// 			*number = history[i]->number;
// 		i++;
// 	}
// }

void		history_change_line(t_history **history, char **line, unsigned *index, char arrow)
{
	(void)history;
	(void)arrow;
	history_clear_line(index, ft_strlen(*line));
	// if (arrow == ARROW_UP && history[0] != NULL)
	// {
	// }
	// else if (arrow == ARROW_DOWN)
	// {
	// }
	// else
		ft_printf("\a");
	ft_putstr(*line);
	*index = ft_strlen(*line);
}
