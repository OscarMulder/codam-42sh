/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   input_parse_ctrl_down.c                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: rkuijper <rkuijper@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/05/17 11:50:51 by rkuijper       #+#    #+#                */
/*   Updated: 2019/08/19 14:43:18 by rkuijper      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"
#include <sys/ioctl.h>

/*
**	Moves the cursor (and index) down or sets it at end if it would otherwise
**	get out of bounds.
*/

void		curs_move_down(t_inputdata *data, t_vshdata *vshdata)
{
	struct winsize	ws; //WILL BE OSCARS DATA

	ioctl(STDIN_FILENO, TIOCGWINSZ, &ws); //WILL BE OSCARS DATA
	if (data->index == data->len_cur)
		return ;
	else if (data->len_cur - data->index < ws.ws_col)
		curs_go_end(data, vshdata);
	else
	{
		ft_printf(CURS_DOWN);
		data->index += ws.ws_col;
		data->coords.y++;
	}
}
