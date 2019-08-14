/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   input_parse_backspace.c                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: rkuijper <rkuijper@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/05/16 13:43:07 by rkuijper       #+#    #+#                */
/*   Updated: 2019/08/14 11:24:41 by rkuijper      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"
#include <term.h>
#include <sys/ioctl.h>

/*
**	Backspaces are handled saving the cursor position and then clearing the
**	screen and then reprinting the edited line and then going back to the old
**	cursor position.
*/

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

int		input_handle_backspace(t_inputdata *data, t_vshdata *vshdata)
{
	struct winsize	ws;

	ioctl(STDIN_FILENO, TIOCGWINSZ, &ws);
	if (data->index > 0)
	{
		input_clear_char_at(&vshdata->line, data->index - 1);
		data->len_cur--;
		curs_move_left(data);
		ft_putstr("\e[s");
		ft_iputstr(vshdata->line + data->index, get_cursor_linepos(), ws.ws_col);
		ft_putchar(' ');
		ft_putstr("\e[u");
	}
	return (FUNCT_SUCCESS);
}
