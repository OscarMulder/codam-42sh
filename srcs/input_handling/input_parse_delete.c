/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   input_parse_delete.c                               :+:    :+:            */
/*                                                     +:+                    */
/*   By: omulder <omulder@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/05/16 13:44:53 by rkuijper       #+#    #+#                */
/*   Updated: 2019/08/14 10:50:03 by rkuijper      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"
#include <term.h>
#include <sys/ioctl.h>

/*
**	Real line gets updated, then the cursor position is saved (DOESNT WORK WITH RESIZING)
**	Lines will be cleared and everything will be reprinted (sadly).
*/

/*
**	Functionality is subject to change. There *should* be a way to do this more easily
**	using ncurses and tgetstr("dc") funcionality, but I have yet to find it.
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

int			input_handle_delete(t_inputdata *data, t_vshdata *vshdata)
{
	struct winsize	ws;
	
	ioctl(STDIN_FILENO, TIOCGWINSZ, &ws);
	if (data->index < data->len_cur)
	{
		input_clear_char_at(&vshdata->line, data->index);
		data->len_cur--;
		ft_putstr("\e[s");
		ft_iputstr(vshdata->line + data->index, get_cursor_linepos(), ws.ws_col);
		ft_putchar(' ');
		ft_putstr("\e[u");
	}
	return (FUNCT_SUCCESS);
}
