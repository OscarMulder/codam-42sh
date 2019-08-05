/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   history_change_line.c                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: omulder <omulder@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/06/02 14:28:54 by mavan-he       #+#    #+#                */
/*   Updated: 2019/08/05 11:51:22 by jbrinksm      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"
#include "libft.h"

# define TC_GETCURSORPOS "\e[6n"

static int	get_cursor_linepos()
{
	char	*response;
	char	*buf;
	int		ret;

	buf = ft_strnew(10);
	ft_putstr_fd(TC_GETCURSORPOS, STDIN_FILENO);
	while (ret != 0)
	{
		ret = read(STDIN_FILENO, buf, 10);
		if (ret > 0)
		{
			response = ft_strdup(buf);
		}
	}
}

/*
** Unfinished, since there is no multiline support yet. Works with single lines.
*/
static void	history_clear_line(t_vshdata *vshdata, unsigned *index, unsigned linelen)
{
	(void)vshdata;
	ft_printf("\e[6n\n");
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
		char arrow)
{
	history_clear_line(vshdata, &(data->index), ft_strlen(vshdata->line));
	if (arrow == ARROW_UP)
	{
		if (history_index_change_up(data))
			set_line(data, &vshdata->line);
		else
			ft_printf("\a");
	}
	else if (arrow == ARROW_DOWN)
	{
		if (history_index_change_down(data))
			set_line(data, &vshdata->line);
		else
			ft_bzero(vshdata->line, data->len_max);
	}
	ft_putstr(vshdata->line);
	data->index = ft_strlen(vshdata->line);
	return (FUNCT_SUCCESS);
}
