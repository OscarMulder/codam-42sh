/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   input_parse_ctrl_up.c                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: omulder <omulder@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/05/16 15:03:17 by rkuijper       #+#    #+#                */
/*   Updated: 2019/08/08 20:03:09 by jbrinksm      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"
#include <sys/ioctl.h>

/*
**	Moves the cursor (and index) up or sets it at home if it would otherwise
**	collide with the prompt.
*/

void		curs_move_up(t_inputdata *data, t_vshdata *vshdata)
{
	struct winsize	ws; //WILL BE OSCARS DATA
	size_t			linepos;

	ioctl(STDIN_FILENO, TIOCGWINSZ, &ws); //WILL BE OSCARS DATA
	linepos = get_cursor_linepos();
	if (data->index == 0)
		return ;
	else if (data->index < ws.ws_col)
		curs_go_home(data, vshdata);
	else
	{
		ft_printf(CURS_UP);
		data->index -= ws.ws_col;
	}
}
