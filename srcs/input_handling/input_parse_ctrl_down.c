/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   input_parse_ctrl_down.c                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: rkuijper <rkuijper@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/05/17 11:50:51 by rkuijper       #+#    #+#                */
/*   Updated: 2019/08/26 15:22:23 by rkuijper      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"
#include <sys/ioctl.h>

/*
**	Moves the cursor (and index) down or sets it at end if it would otherwise
**	get out of bounds.
*/

static unsigned	get_cur_line_index(t_vshdata *data)
{
	int i;

	i = data->index - 1;
	while (i > 0)
	{
		if (data->line[i] == '\n')
			return (data->index - i - 1);
		i--;
	}
	return (data->index);
}

static void	move_down_handle_newline(t_vshdata *data)
{
	unsigned	i;
	int			j;
	unsigned	l;

	i = data->index;
	j = -1;
	l = get_cur_line_index(data, data);
	while (data->line[i] != '\0')
	{
		if (data->line[i] == '\n')
		{
			if (j == -1)
				j = 0;
			else
				break ;
		}
		else if (j != -1)
		{
			if (j >= (int)l)
				break ;
			j++;
		}
		i++;
	}
	curs_move_n_right(data, data, i - data->index);
}

void		curs_move_down(t_vshdata *data)
{
	struct winsize	ws;
	char			*newline_str;

	ioctl(STDIN_FILENO, TIOCGWINSZ, &ws);
	if (data->index == data->len_cur)
		return ;
	newline_str = ft_strchr(data->line + data->index, '\n');
	if (newline_str != NULL)
		move_down_handle_newline(data, data);
	else if (data->len_cur - data->index < ws.ws_col)
		curs_go_end(data, data);
	else
	{
		ft_printf(CURS_DOWN);
		data->index += ws.ws_col;
		data->coords.y++;
	}
}
