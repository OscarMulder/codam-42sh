/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   history_change_line.c                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: omulder <omulder@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/06/02 14:28:54 by mavan-he       #+#    #+#                */
/*   Updated: 2019/08/04 12:06:32 by mavan-he      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"
#include "libft.h"

/*
** Unfinished, since there is no multiline support yet. Works with single lines.
*/
static void	history_clear_line(t_vshdata *vshdata, unsigned *index)
{
	unsigned line_len;

	line_len = ft_strlen(vshdata->line);
	if (*index > 0)
		ft_printf("\e[%dD", *index);
	*index = 0;
	while (*index < line_len)
	{
		ft_putchar(' ');
		(*index)++;
	}
	if (*index > 0)
		ft_printf("\e[%D", *index);
}

static int	malloc_and_copy(t_inputdata *data, char **line, char *str)
{
	int len;

	len = ft_strlen(str);
	if (len < data->len_max)
	{
		ft_bzero(*line, data->len_max);
	}
	else
	{
		data->len_max *= 2;
		ft_strdel(&(*line));
		*line = ft_strnew(data->len_max);
		if (*line == NULL)
			return (FUNCT_ERROR);
	}
	ft_strcpy(*line, str);
	return (FUNCT_SUCCESS);
}

static int	set_line(t_inputdata *data, char **line)
{
	if (malloc_and_copy(data, line, data->history[data->hist_index]->str)
	== FUNCT_ERROR)
		return (FUNCT_ERROR);
	return (FUNCT_SUCCESS);
}

int			history_change_line(t_inputdata *data, t_vshdata *vshdata,
	char **line, char arrow)
{
	history_clear_line(vshdata, &(data->index));
	if (arrow == ARROW_UP)
	{
		if (history_index_change_up(data))
			set_line(data, line);
		else
			ft_printf("\a");
	}
	else if (arrow == ARROW_DOWN)
	{
		if (history_index_change_down(data))
			set_line(data, line);
		else
			ft_bzero(*line, data->len_max);
	}
	ft_putstr(*line);
	data->index = ft_strlen(*line);
	return (FUNCT_SUCCESS);
}
