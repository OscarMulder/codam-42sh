/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   history_change_line.c                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: omulder <omulder@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/06/02 14:28:54 by mavan-he       #+#    #+#                */
/*   Updated: 2019/08/05 17:14:44 by jbrinksm      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"
#include "libft.h"

#define TC_GETCURSORPOS "\e[6n"

/*
**	
*/

static char	*get_cursor_pos(void)
{
	char	*buf;
	int		ret;

	buf = ft_strnew(100);
	ret = read(STDIN_FILENO, buf, 10);
	if (ret == -1)
	{
		ft_strdel(&buf);
		return (NULL);
	}
	return (buf);
}

static int	get_cursor_linepos()
{
	char	*response;
	int		i;

	ft_putstr(TC_GETCURSORPOS);
	response = get_cursor_pos();
	if (response == NULL)
		return (-1);
	i = 0;
	while (response[i] != '\0' && response[i] != ';')
		i++;
	if (response[i] == '\0' || response[i + 1] == '\0'
		|| ft_isdigit(response[i + 1]) == false)
	{
		ft_strdel(&response);
		return (-1);
	}
	return ((unsigned)ft_atoi(&response[i + 1]));
}

#include <sys/ioctl.h>

static void	history_clear_line(unsigned *index, unsigned linelen, int promptsize)
{
	unsigned	linepos;
	unsigned 	lineposindex;
	int			wasran = 0;
	
	struct winsize	ws;
	ioctl(STDIN_FILENO, TIOCGWINSZ, &ws);
	
	(void)index;
	(void)linelen;
	(void)promptsize;

	linepos = get_cursor_linepos() - 1;
	lineposindex = linepos;
	// ft_printf("STATS: linepos: %i --- index %i --- linelen: %i --- windowsize: %i --- promptsize: %i\n", linepos, *index, linelen, ws.ws_col, promptsize);
	
	if ((int)*index + (promptsize - 1) - ws.ws_col > 0)
	{
		if (lineposindex > 0)
		{
			ft_printf("\e[%iD", lineposindex);
			*index -= lineposindex;
			lineposindex = 0;
		}
		while (lineposindex < linepos)
		{
			ft_putchar(' ');
			lineposindex++;
		}
		if (lineposindex > 0)
		{
			ft_printf("\e[%iD", lineposindex);
			lineposindex = 0;
		}
		/* if next is last */
		ft_putstr("\e[A");
		ft_printf("\e[%iC", promptsize);
		ft_printf("\e[%iC", *index);

		wasran = true;
	}

	/* if we are on final line */
	if ((int)*index + (promptsize - 1) - ws.ws_col <= 0)
	{
		lineposindex = *index;
		if (wasran == true)
			lineposindex -= 2;
		linepos = lineposindex;


		if (lineposindex > 0)
		{
			ft_printf("\e[%iD", lineposindex);
			lineposindex = 0;
		}
		while (lineposindex <= linepos)
		{
			ft_putchar(' ');
			lineposindex++;
		}
		if (lineposindex > 0)
		{
			ft_printf("\e[%iD", lineposindex);
			lineposindex = 0;
		}
	}
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
	history_clear_line(&(data->index), ft_strlen(vshdata->line), vshdata->prompt_len);
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
