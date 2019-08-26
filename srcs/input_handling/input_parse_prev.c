/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   input_parse_prev.c                                 :+:    :+:            */
/*                                                     +:+                    */
/*   By: omulder <omulder@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/05/16 13:39:59 by rkuijper       #+#    #+#                */
/*   Updated: 2019/08/26 12:55:46 by rkuijper      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"
#include <sys/ioctl.h>

/*
**	Algorithm that moves the cursor (and index) to the beginning of a previous
**	word (or beginning of the line if there is none).
*/

void		curs_move_prev_word(t_inputdata *data, t_vshdata *vshdata)
{
	int	i;

	if (data->index == 0)
		return ;
	i = 0;
	if ((data->index > 0
		&& tools_isprintnotblank(vshdata->line[data->index]) == true // i++ if at beginning of previous word
		&& ft_isblank(vshdata->line[data->index - 1]) == true)
		|| (data->index > 0 && data->index == data->len_cur)) // i++ if at end of line
		i++;
	while (data->index - i > 0 // i++ blanks
		&& ft_isblank(vshdata->line[data->index - i]) == true)
		i++;
	if (data->index - i == 0)
		curs_move_n_left(data, vshdata, i);
	else
	{
		while (data->index - i > 0
			&& tools_isprintnotblank(vshdata->line[data->index - i - 1]))
			i++;
		curs_move_n_left(data, vshdata, i);
	}
}

/*
**	`ws` will be taken from `data` after Oscar is done.
**
**	Calculations to move n times to the left (or up if necessar) on the current
**	ws. If used after some weird screen clearing, make sure to compensate
**	for the automatic `index` change if necessar
*/

static void	move_left_parse_newline(t_inputdata *data, t_vshdata *vshdata)
{
	char *pos = ft_strrnchr(vshdata->line, '\n', data->index);
	int len = data->index;
	if (pos != NULL)
		len = (data->index - 1) - (pos - vshdata->line);
	ft_putstr("\e[A");
	if (len > 1)
		ft_printf("\e[%iC", len);
	data->coords.x = len == 1 ? len : len + 1;
	data->coords.y--;
	if (data->coords.y == 1)
	{
		ft_printf("\e[%iC", vshdata->prompt_len);
		data->coords.x += vshdata->prompt_len;
	}
}

static void	move_left_to_colmax(t_inputdata *data, int colmax)
{
	if (data->coords.x == 1)
	{
		data->coords.x = colmax;
		data->coords.y--;
		ft_printf("\e[A\e[%iC", data->coords.x);
	}
	else
	{
		ft_putstr("\e[D");
		data->coords.x--;
	}
}

void		curs_move_n_left(t_inputdata *data, t_vshdata *vshdata, size_t n)
{
	struct winsize	ws;

	if (n <= 0 || data->index == 0)
		return ;
	if (n > data->index)
		n = data->index;
	ioctl(STDIN_FILENO, TIOCGWINSZ, &ws);
	while (n > 0)
	{
		n--;
		data->index--;
		if (vshdata->line[data->index] == '\n')
			move_left_parse_newline(data, vshdata);
		else
			move_left_to_colmax(data, ws.ws_col);
	}
	ft_eprintf("New cursor coordinates: [%d:%d]\n", data->coords.x, data->coords.y);
}

/*
**	`ws` will be taken from data after Oscar is done.
**
**	Moves the cursor (and index) one to the left (or up if necessary)
**	If used after some weird screen clearing, make sure to compensate
**	for the automatic `index` change if necessar
*/

void		curs_move_left(t_inputdata *data, t_vshdata *vshdata)
{
	if (data->index > 0)
		curs_move_n_left(data, vshdata, 1);
}
