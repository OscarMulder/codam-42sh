/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   input_parse_prev.c                                 :+:    :+:            */
/*                                                     +:+                    */
/*   By: omulder <omulder@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/05/16 13:39:59 by rkuijper       #+#    #+#                */
/*   Updated: 2019/08/26 18:20:16 by jbrinksm      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"
#include <sys/ioctl.h>

/*
**	Algorithm that moves the cursor (and index) to the beginning of a previous
**	word (or beginning of the line if there is none).
*/

void		curs_move_prev_word(t_vshdata *data)
{
	int	i;

	if (data->line->index == 0)
		return ;
	i = 0;
	if ((data->line->index > 0
		&& tools_isprintnotblank(data->line->line[data->line->index]) == true // i++ if at beginning of previous word
		&& ft_isblank(data->line->line[data->line->index - 1]) == true)
		|| (data->line->index > 0 && data->line->index == data->line->len_cur)) // i++ if at end of line
		i++;
	while (data->line->index - i > 0 // i++ blanks
		&& ft_isblank(data->line->line[data->line->index - i]) == true)
		i++;
	if (data->line->index - i == 0)
		curs_move_n_left(data, i);
	else
	{
		while (data->line->index - i > 0
			&& tools_isprintnotblank(data->line->line[data->line->index - i - 1]))
			i++;
		curs_move_n_left(data, i);
	}
}

/*
**	`ws` will be taken from `data` after Oscar is done.
**
**	Calculations to move n times to the left (or up if necessar) on the current
**	ws. If used after some weird screen clearing, make sure to compensate
**	for the automatic `index` change if necessar
*/

static void	move_left_parse_newline(t_vshdata *data)
{
	char *pos = ft_strrnchr(data->line->line, '\n', data->line->index);
	int len = data->line->index;
	if (pos != NULL)
		len = (data->line->index - 1) - (pos - data->line->line);
	ft_putstr("\e[A");
	if (len > 1)
		ft_printf("\e[%iC", len);
	data->curs->coords.x = len == 1 ? len : len + 1;
	data->curs->coords.y--;
	if (data->curs->coords.y == 1)
	{
		ft_printf("\e[%iC", data->prompt->prompt_len);
		data->curs->coords.x += data->prompt->prompt_len;
	}
}

static void	move_left_to_colmax(t_vshdata *data, int colmax)
{
	if (data->curs->coords.x == 1)
	{
		data->curs->coords.x = colmax;
		data->curs->coords.y--;
		ft_printf("\e[A\e[%iC", data->curs->coords.x);
	}
	else
	{
		ft_putstr("\e[D");
		data->curs->coords.x--;
	}
}

void		curs_move_n_left(t_vshdata *data, size_t n)
{
	struct winsize	ws;

	if (n <= 0 || data->line->index == 0)
		return ;
	if (n > data->line->index)
		n = data->line->index;
	ioctl(STDIN_FILENO, TIOCGWINSZ, &ws);
	while (n > 0)
	{
		n--;
		data->line->index--;
		if (data->line->line[data->line->index] == '\n')
			move_left_parse_newline(data);
		else
			move_left_to_colmax(data, ws.ws_col);
	}
	ft_eprintf("New cursor coordinates: [%d:%d]\n", data->curs->coords.x, data->curs->coords.y);
}

/*
**	`ws` will be taken from data after Oscar is done.
**
**	Moves the cursor (and index) one to the left (or up if necessary)
**	If used after some weird screen clearing, make sure to compensate
**	for the automatic `index` change if necessar
*/

void		curs_move_left(t_vshdata *data)
{
	if (data->line->index > 0)
		curs_move_n_left(data, 1);
}
