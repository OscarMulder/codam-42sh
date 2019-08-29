/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   input_parse_next.c                                 :+:    :+:            */
/*                                                     +:+                    */
/*   By: omulder <omulder@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/05/16 13:41:00 by rkuijper       #+#    #+#                */
/*   Updated: 2019/08/29 14:25:55 by jbrinksm      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"
#include <sys/ioctl.h>

/*
**	Algorithm that moves the cursor (and index) to the beginning of the next
**	word (or end of the line if there is none).
*/

void		curs_move_next_word(t_vshdata *data)
{
	size_t	i;

	if (data->line->index == data->line->len_cur)
		return ;
	i = 0;
	while (ft_isprint(data->line->line[data->line->index + i]) == true
		&& ft_isblank(data->line->line[data->line->index + i]) == false)
		i++;
	while (ft_isblank(data->line->line[data->line->index + i]) == true)
		i++;
	if ((data->line->index + i == data->line->len_cur) // end of line
		|| (ft_isprint(data->line->line[data->line->index + i]) == true
		&& ft_isblank(data->line->line[data->line->index + i]) == false))
		curs_move_n_right(data, i);
}

/*
**	`ws` will be taken from `data` after Oscar is done.
**
**	Calculations to move cursor (and index) n times to the right
**	(or up if necessary) on the current ws.
**	If used after some weird screen clearing, make sure to compensate
**	for the automatic `index` change if necessary.
*/

static void	move_right_parse_newline(t_vshdata *data)
{
	char *pos = ft_strrnchr(data->line->line, '\n', data->line->index);
	int len = data->line->index + data->prompt->prompt_len;
	if (pos != NULL)
		len = (data->line->index - 1) - (pos - data->line->line);
	ft_putstr("\e[B");
	if (len > 1)
		ft_printf("\e[%iD", len);
	data->curs->coords.x = 1;
	data->curs->coords.y++;
}

void		curs_move_right_at_colmax(t_vshdata *data, int colmax)
{
	if (data->curs->coords.x == colmax)
	{
		data->curs->coords.x = 1;
		data->curs->coords.y++;
		ft_printf("\e[B\e[%iD", colmax);
	}
	else
	{
		ft_putstr("\e[C");
		data->curs->coords.x++;
	}
}

void		curs_move_n_right(t_vshdata *data, size_t n)
{
	if (n <= 0 || data->line->index == data->line->len_cur)
		return ;
	if (n > data->line->len_cur - data->line->index)
		n = data->line->len_cur - data->line->index;
	while (n > 0)
	{
		if (data->line->line[data->line->index] == '\n' && data->line->index != data->line->len_cur - 1) // LET ME KNOW IF THIS IS FINE @ROB -Jorn
		{
			move_right_parse_newline(data);
		}
		else
			curs_move_right_at_colmax(data, data->curs->cur_ws_col);
		n--;
		data->line->index++;
	}
}

/*
**	`ws` will be taken from data after Oscar is done.
**
**	Moves the cursor (and index) one to the right (or down if necessary)
**	If used after some weird screen clearing, make sure to compensate
**	for the automatic `index` change if necessary.
*/

void		curs_move_right(t_vshdata *data)
{
	curs_move_n_right(data, 1);
}
