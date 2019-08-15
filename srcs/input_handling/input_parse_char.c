/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   input_parse_char.c                                 :+:    :+:            */
/*                                                     +:+                    */
/*   By: omulder <omulder@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/05/16 13:33:54 by rkuijper       #+#    #+#                */
/*   Updated: 2019/08/15 11:32:16 by rkuijper      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"
#include <termcap.h>

/*
**	This function makes sure that when you are inserting a char
**	inbetween other chars on the line, that the chars on the right
**	side are moved over by 1, which creates a gap for the char
**	that needs to be inserted.
*/

static void	create_char_gap(char *line, int len, int gap_index)
{
	int	i;

	i = len - 1;
	while (i >= gap_index)
	{
		line[i + 1] = line[i];
		i--;
	}
}

/*
**	Inserts a char `c` at `index` on `*line`.
**	When there is not enough space on `*line`, the amount
**	of memory allocated for `*line` is doubled first.
*/

static int	add_char_at(t_inputdata *data, char **line)
{
	char		*tmp;
	int			len;

	len = data->len_cur;
	if (len < data->len_max)
	{
		create_char_gap(*line, len, data->index);
		(*line)[data->index] = data->c;
	}
	else
	{
		data->len_max *= 2;
		tmp = ft_strnew(data->len_max);
		if (tmp == NULL)
			return (FUNCT_ERROR);
		ft_strcpy(tmp, *line);
		ft_strdel(line);
		create_char_gap(tmp, len, data->index);
		tmp[data->index] = data->c;
		*line = tmp;
	}
	data->len_cur++;
	return (FUNCT_SUCCESS);
}

/*
**	Makes sure that when a `\n` char is caught, it is put
**	at the end of `*line`. When there is not enough space
**	on line, the amount of memory for `*line` is increased
**	by 1 byte first.
*/

static int	add_newline(t_inputdata *data, char **line)
{
	char		*tmp;
	int			len;

	len = data->len_cur;
	if (len < data->len_max)
		(*line)[len] = '\n';
	else
	{
		data->len_max += 1;
		tmp = ft_strnew(data->len_max);
		if (tmp == NULL)
			return (FUNCT_ERROR);
		ft_strcpy(tmp, *line);
		ft_strdel(line);
		tmp[len] = '\n';
		*line = tmp;
	}
	data->len_cur++;
	return (FUNCT_SUCCESS);
}

int			ft_tputchar(int c)
{
	write(1, &c, 1);
	return (1);
}

static void	ft_iputstr(char *str, int linepos, int maxcol)
{
	int		i;

	i = 0;
	while (str[i] != '\0')
	{
		ft_putchar(str[i]);
		if (linepos == maxcol)
		{
			linepos = 0;
			ft_putchar('\n');
		}
		else
			linepos++;
		i++;
	}
}

/*
**	`ws` will be taken from data when Oscars resize function is finished.
**
**	A the string will be edited and reprinted from the point of insertion.
*/
#include <sys/ioctl.h>

int			input_parse_char(t_inputdata *data, t_vshdata *vshdata)
{
	struct winsize ws;
	
	ioctl(STDIN_FILENO, TIOCGWINSZ, &ws);
	if (ft_isprint(data->c))
	{
		if (add_char_at(data, &vshdata->line) == FUNCT_ERROR)
			return (FUNCT_ERROR);
		ft_putstr("\e[s");
		ft_iputstr(vshdata->line + data->index, get_cursor_linepos(), ws.ws_col);
		ft_putstr("\e[u");
		curs_move_right(data);
	}
	else if (data->c == '\n')
	{
		if (add_newline(data, &vshdata->line) == FUNCT_ERROR)
			return (FUNCT_ERROR);
	}
	return (FUNCT_SUCCESS);
}
