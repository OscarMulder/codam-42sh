/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   input_parse_backspace.c                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: rkuijper <rkuijper@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/05/16 13:43:07 by rkuijper       #+#    #+#                */
/*   Updated: 2019/08/15 10:37:12 by rkuijper      ########   odam.nl         */
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

int		input_handle_backspace(t_inputdata *data, t_vshdata *vshdata)
{
	struct winsize	ws;

	ioctl(STDIN_FILENO, TIOCGWINSZ, &ws);
	if (data->index > 0)
	{
		input_clear_char_at(&vshdata->line, data->index - 1);
		data->len_cur--;
		curs_move_left(data);
		ft_printf("\e[s%s \e[u", vshdata->line + data->index);
	}
	return (FUNCT_SUCCESS);
}
