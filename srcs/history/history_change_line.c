/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   history_change_line.c                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: omulder <omulder@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/06/02 14:28:54 by mavan-he       #+#    #+#                */
/*   Updated: 2019/08/06 15:40:06 by jbrinksm      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"
#include "libft.h"

#define TC_GETCURSORPOS "\e[6n"

/*
**	
*/

// static char	*get_cursor_pos(void)
// {
// 	char	*buf;
// 	int		ret;

// 	buf = ft_strnew(100);
// 	ret = read(STDIN_FILENO, buf, 10);
// 	if (ret == -1)
// 	{
// 		ft_strdel(&buf);
// 		return (NULL);
// 	}
// 	return (buf);
// }

// static int	get_cursor_linepos()
// {
// 	char	*response;
// 	int		i;

// 	ft_putstr(TC_GETCURSORPOS);
// 	response = get_cursor_pos();
// 	if (response == NULL)
// 		return (-1);
// 	i = 0;
// 	while (response[i] != '\0' && response[i] != ';')
// 		i++;
// 	if (response[i] == '\0' || response[i + 1] == '\0'
// 		|| ft_isdigit(response[i + 1]) == false)
// 	{
// 		ft_strdel(&response);
// 		return (-1);
// 	}
// 	return ((unsigned)ft_atoi(&response[i + 1]));
// }

#include <sys/ioctl.h>

static void	history_clear_line_resetcursor(unsigned *index, int linelen, int promptsize)
{
	struct winsize	ws;
	int				promptlinelen;
	int				currentline;

	ioctl(STDIN_FILENO, TIOCGWINSZ, &ws);
	promptlinelen = ws.ws_col - promptsize;
	if ((int)*index < linelen)
	{
		/* only one line (if the line is exactly full, the cursor stays at
		the last char instead of going to a new line */
		if (linelen + promptsize <= ws.ws_col)
		{
			sleep(1);
			ft_printf("\e[%iC", linelen - (int)*index);
			*index += (linelen - (int)*index);
			sleep(1);
		}
		else
		{
			if ((int)*index < promptlinelen)
			{
				sleep(1);
				ft_printf("\e[%iC", promptlinelen - (int)*index);
				*index += (promptlinelen - (int)*index);
				sleep(1);
				ft_printf("\e[B");
				*index += 1;
				sleep(1);
				ft_printf("\e[%iD", ws.ws_col - 1);
				sleep(1);
			}
			while ((int)*index < linelen)
			{
				currentline = linelen - *index % ws.ws_col;
				ft_printf("\e[%iC", currentline);
				ft_printf("%i", currentline);
				*index += currentline;
				// ft_printf("%i %i", currentline, ws.ws_col);
				sleep(1);
				break ;
			}
		}
		// else
		// {
		// 	while (linele)
		// }
	}
}

static void	history_clear_line(unsigned *index, int linelen, int promptsize)
{
	// int		currentlinelen;
	// int		cursorindex;

	history_clear_line_resetcursor(index, linelen, promptsize);
	// if (linelen + promptsize >= ws.ws_col)
	// {
	// }
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
