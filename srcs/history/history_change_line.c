/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   history_change_line.c                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: mavan-he <mavan-he@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/06/02 14:28:54 by mavan-he       #+#    #+#                */
/*   Updated: 2019/07/26 17:34:46 by omulder       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"
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

void		history_change_line(t_inputdata *data, char **line, char arrow)
{
	history_clear_line(&(data->index), ft_strlen(*line));
	if (arrow == ARROW_UP && (data->hist_index - 1) >= 0 && data->history[(data->hist_index - 1)] != NULL)
	{
		data->hist_index -= 1;
		*line = ft_strdup(data->history[data->hist_index]->str);
	}
	else if (arrow == ARROW_DOWN && (data->hist_index + 1) <= HISTORY_MAX && data->history[(data->hist_index + 1)]->str != NULL)
	{
		data->hist_index += 1;
		*line = ft_strdup(data->history[data->hist_index]->str);
	}
	else if (arrow == ARROW_DOWN)
	{
		if (data->history[(data->hist_index)]->str != NULL)
			data->hist_index += 1;
		*line = ft_strdup("");
	}
	else
		ft_printf("\a");
	ft_putstr(*line);
	data->index = ft_strlen(*line);
}
