/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   input_parse_ctrl_down.c                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: rkuijper <rkuijper@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/05/17 11:50:51 by rkuijper       #+#    #+#                */
/*   Updated: 2019/08/08 16:36:06 by jbrinksm      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"
#include <sys/ioctl.h>

void		curs_move_down(t_inputdata *data, t_vshdata *vshdata)
{
	struct winsize	ws; //WILL BE OSCARS DATA
	size_t			linelen;

	ioctl(STDIN_FILENO, TIOCGWINSZ, &ws); //WILL BE OSCARS DATA
	linelen = ft_strlen(vshdata->line);
	if (data->index == linelen)
		return ;
	else if (linelen - data->index < ws.ws_col)
		curs_go_end(data, vshdata);
	else
	{
		ft_printf(CURS_DOWN);
		data->index += ws.ws_col;
	}
}
