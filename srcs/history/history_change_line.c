/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   history_change_line.c                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: mavan-he <mavan-he@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/06/02 14:28:54 by mavan-he       #+#    #+#                */
/*   Updated: 2019/06/02 16:23:30 by mavan-he      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"
#include "vsh_history.h"
#include "libft.h"

static void	history_clear_line(unsigned *index, unsigned linelen)
{
	ft_printf("\e[%dD", (*index) - 1);
	*index = 0;
	while (*index < linelen)
	{
		ft_putchar(' ');
		(*index)++;
	}
	ft_printf("\e[%dD", linelen - 1);
}

void		history_change_line(char **line, unsigned *index, char arrow)
{
	history_clear_line(index, ft_strlen(*line));
	if (arrow == ARROW_UP && history_tmp != 0)
	{
		history_tmp--;
		*line = history[history_tmp];
	}
	else if (arrow == ARROW_DOWN && history_tmp != history_i % 500)
	{
		*line = history[history_tmp];
		history_tmp++;
	}
	else
		ft_printf("\a");
	ft_putstr(*line);
	*index = ft_strlen(*line);
}
