/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   input_parse_ctrl_up.c                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: omulder <omulder@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/05/16 15:03:17 by rkuijper       #+#    #+#                */
/*   Updated: 2019/08/26 15:31:17 by rkuijper      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"
#include <sys/ioctl.h>

/*
**	Moves the cursor (and index) up or sets it at home if it would otherwise
**	collide with the prompt.
*/

static unsigned	get_cur_line_index(t_inputdata *data, t_vshdata *vshdata)
{
	int i;

	i = data->index - 1;
	while (i > 0)
	{
		if (vshdata->line[i] == '\n')
			return (data->index - i - 1);
		i--;
	}
	return (data->index);
}

static void	move_up_handle_newline(t_inputdata *data, t_vshdata *vshdata)
{
	unsigned	i;
	int			j;
	unsigned	l;

	i = data->index;
	j = -1;
	l = get_cur_line_index(data, vshdata);
	ft_eprintf("Line index: %d\n", l);
	while (i > 0)
	{
		if (vshdata->line[i] == '\n')
		{
			if (j == -1)
				j = 0;
			else
			{
				j = 1;
				break;
			}
		}
		i--;
	}
	if (j != -1)
		i += l + (j == 1 ? 1 : 0);
	curs_move_n_left(data, vshdata, data->index - i);
}

void		curs_move_up(t_inputdata *data, t_vshdata *vshdata)
{
	struct winsize	ws;
	char			*newline_str;

	ioctl(STDIN_FILENO, TIOCGWINSZ, &ws);
	if (data->index == 0)
		return ;
	newline_str = ft_strrnchr(vshdata->line, '\n', data->index);
	if (newline_str != NULL)
		move_up_handle_newline(data, vshdata);
	else if (data->index < ws.ws_col)
		curs_go_home(data, vshdata);
	else
	{
		ft_printf(CURS_UP);
		data->index -= ws.ws_col;
		data->coords.y--;
	}
}
