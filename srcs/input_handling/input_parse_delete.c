/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   input_parse_delete.c                               :+:    :+:            */
/*                                                     +:+                    */
/*   By: omulder <omulder@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/05/16 13:44:53 by rkuijper       #+#    #+#                */
/*   Updated: 2019/08/15 12:18:35 by rkuijper      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"
#include <term.h>
#include <sys/ioctl.h>

/*
**	Real line gets updated, then the cursor position is saved (DOESNT WORK WITH RESIZING)
**	Lines will be cleared and everything will be reprinted (sadly).
*/

int			input_handle_delete(t_inputdata *data, t_vshdata *vshdata)
{
	struct winsize	ws;
	
	ioctl(STDIN_FILENO, TIOCGWINSZ, &ws);
	if (data->index < data->len_cur)
	{
		input_clear_char_at(&vshdata->line, data->index);
		data->len_cur--;
		// The following is highly illegal and needs to be better.
		// The space is currently the only thing 'removing' the deleted char
		// so this functionality doesn't account for removing a newline char.
		ft_printf("\e[s%s \e[u", vshdata->line + data->index);
	}
	else
		ft_putchar('\a');
	return (FUNCT_SUCCESS);
}
