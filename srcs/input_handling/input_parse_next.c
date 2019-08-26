/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   input_parse_next.c                                 :+:    :+:            */
/*                                                     +:+                    */
/*   By: omulder <omulder@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/05/16 13:41:00 by rkuijper       #+#    #+#                */
/*   Updated: 2019/08/26 11:35:39 by rkuijper      ########   odam.nl         */
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

	if (data->index == data->len_cur)
		return ;
	i = 0;
	while (ft_isprint(data->line[data->index + i]) == true
		&& ft_isblank(data->line[data->index + i]) == false)
		i++;
	while (ft_isblank(data->line[data->index + i]) == true)
		i++;
	if ((data->index + i == data->len_cur) // end of line
		|| (ft_isprint(data->line[data->index + i]) == true
		&& ft_isblank(data->line[data->index + i]) == false))
		curs_move_n_right(data, data, i);
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
	char *pos = ft_strrnchr(data->line, '\n', data->index);
	int len = data->index + data->prompt_len;
	if (pos != NULL)
		len = (data->index - 1) - (pos - data->line);
	ft_putstr("\e[B");
	if (len > 1)
		ft_printf("\e[%iD", len);
	data->coords.x = 1;
	data->coords.y++;
}

static void	move_right_at_colmax(t_vshdata *data, int colmax)
{
	if (data->coords.x == colmax)
	{
		data->coords.x = 1;
		data->coords.y++;
		ft_printf("\e[B\e[%iD", colmax);
	}
	else
	{
		ft_putstr("\e[C");
		data->coords.x++;
	}
}

void		curs_move_n_right(t_vshdata *data, size_t n)
{
	struct winsize	ws;

	if (n <= 0 || data->index == data->len_cur)
		return ;
	if (n > data->len_cur - data->index)
		n = data->len_cur - data->index;
	ioctl(STDIN_FILENO, TIOCGWINSZ, &ws);
	while (n > 0)
	{
		if (data->line[data->index] == '\n')
			move_right_parse_newline(data, data);
		else
			move_right_at_colmax(data, ws.ws_col);
		n--;
		data->index++;
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
	if (data->index < data->len_cur)
		curs_move_n_right(data, data, 1);
}