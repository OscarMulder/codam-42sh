/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   input_parse_char.c                                 :+:    :+:            */
/*                                                     +:+                    */
/*   By: omulder <omulder@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/05/16 13:33:54 by rkuijper       #+#    #+#                */
/*   Updated: 2019/08/08 20:08:11 by jbrinksm      ########   odam.nl         */
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

	i = len;
	i--;
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

static int	add_char_at(char **line, int index, char c, int *len_max)
{
	char		*tmp;
	int			len;

	len = ft_strlen(*line);
	if (len < *len_max)
	{
		create_char_gap(*line, len, index);
		(*line)[index] = c;
	}
	else
	{
		*len_max *= 2;
		tmp = ft_strnew(*len_max);
		if (tmp == NULL)
			return (FUNCT_ERROR);
		ft_strcpy(tmp, *line);
		ft_strdel(line);
		create_char_gap(tmp, len, index);
		tmp[index] = c;
		*line = tmp;
	}
	return (FUNCT_SUCCESS);
}

/*
**	Makes sure that when a `\n` char is caught, it is put
**	at the end of `*line`. When there is not enough space
**	on line, the amount of memory for `*line` is increased
**	by 1 byte first.
*/

static int	add_newline(char **line, int *len_max)
{
	char		*tmp;
	int			len;

	len = ft_strlen(*line);
	if (len < *len_max)
		(*line)[len] = '\n';
	else
	{
		*len_max += 1;
		tmp = ft_strnew(*len_max);
		if (tmp == NULL)
			return (FUNCT_ERROR);
		ft_strcpy(tmp, *line);
		ft_strdel(line);
		tmp[len] = '\n';
		*line = tmp;
	}
	return (FUNCT_SUCCESS);
}

int			ft_tputchar(int c)
{
	write(1, &c, 1);
	return (1);
}

/*
**	This function will take care of the printing of the line.
**	When it is at the last column in the terminal, it will print a `\n` to
**	make room.
*/

static void	ft_iputstr(char *str, int linepos, int maxcol)
{
	int				i;

	i = 0;
	while (str[i] != '\0')
	{
		ft_putchar(str[i]);
		ft_eprintf("%i/%i\n", linepos, maxcol); //DEBUG PRINT
		if (linepos == maxcol)
		{
			linepos = 0;
			ft_putchar('\n');
		}
		i++;
		linepos++;
	}
}

#include <sys/ioctl.h>

/*
**	`ws` will be taken from data when Oscars resize function is finished.
**
**	A the string will be edited and reprinted from the point of insertion.
*/

int			input_parse_char(t_inputdata *data, char **line)
{
	struct winsize	ws; //WILL BE OSCARS DATA
	int				linepos;
	unsigned len;

	ioctl(STDIN_FILENO, TIOCGWINSZ, &ws); //WILL BE OSCARS DATA
	linepos = get_cursor_linepos(); //WILL BE OSCARS DATA
	if (ft_isprint(data->c))
	{
		if (add_char_at(line, data->index, data->c, &(data->len_max))
		== FUNCT_ERROR)
			return (FUNCT_ERROR);
		len = ft_strlen(*line + data->index);
		ft_iputstr(*line + data->index, linepos, ws.ws_col);
		data->index += len; // compensate index for automatic reposition because of the putstr
		curs_move_n_left(data, len); // we don't want the cursor to be displayed at the end of the print
		curs_move_right(data, *line); // move right after char has been printed
	}
	else if (data->c == '\n')
	{
		if (add_newline(line, &(data->len_max)) == FUNCT_ERROR)
			return (FUNCT_ERROR);
	}
	return (FUNCT_SUCCESS);
}
