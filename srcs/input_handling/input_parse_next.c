/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   input_parse_next.c                                 :+:    :+:            */
/*                                                     +:+                    */
/*   By: omulder <omulder@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/05/16 13:41:00 by rkuijper       #+#    #+#                */
/*   Updated: 2019/08/12 14:36:49 by rkuijper      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"
#include <sys/ioctl.h>

/*
**	Algorithm that moves the cursor (and index) to the beginning of the next
**	word (or end of the line if there is none).
*/

void		curs_move_next_word(t_inputdata *data, t_vshdata *vshdata)
{
	size_t	i;

	if (data->index == data->len_cur)
		return ;
	i = 0;
	while (ft_isprint(vshdata->line[data->index + i]) == true
		&& ft_isblank(vshdata->line[data->index + i]) == false)
		i++;
	while (ft_isblank(vshdata->line[data->index + i]) == true)
		i++;
	if ((data->index + i == data->len_cur) // end of line
		|| (ft_isprint(vshdata->line[data->index + i]) == true
		&& ft_isblank(vshdata->line[data->index + i]) == false))
		curs_move_n_right(data, vshdata, i);
}

/*
**	`ws` will be taken from `data` after Oscar is done.
**
**	Calculations to move cursor (and index) n times to the right
**	(or up if necessary) on the current ws.
**	If used after some weird screen clearing, make sure to compensate
**	for the automatic `index` change if necessary.
*/

void		curs_move_n_right(t_inputdata *data, t_vshdata *vshdata, size_t n)
{
	struct winsize	ws; //WILL BE OSCARS DATA
	int				linepos;
	int				down;
	int				x_offset;

	(void)vshdata;
	if (n == 0 || data->len_cur == data->index)
		return ;
	if (n > data->len_cur - data->index)
		n = data->len_cur - data->index;
	ioctl(STDIN_FILENO, TIOCGWINSZ, &ws); //WILL BE OSCARS DATA
	linepos = get_cursor_linepos();
	down = ((linepos - 1) + n) / ws.ws_col;
	x_offset = (((linepos - 1) + n) % ws.ws_col) - (linepos - 1);
	if (down > 0)
		ft_printf("\e[%iB", down);
	if (x_offset > 0)
		ft_printf("\e[%iC", x_offset);
	else if (x_offset < 0)
		ft_printf("\e[%iD", x_offset * -1);
	data->index += n;
}

/*
**	`ws` will be taken from data after Oscar is done.
**
**	Moves the cursor (and index) one to the right (or down if necessary)
**	If used after some weird screen clearing, make sure to compensate
**	for the automatic `index` change if necessary.
*/

void		curs_move_right(t_inputdata *data)
{
	struct winsize	ws; //WILL BE OSCARS DATA

	ioctl(STDIN_FILENO, TIOCGWINSZ, &ws); //WILL BE OSCARS DATA
	ft_eprintf("R BEF LINEPOS: %i/%i\n", get_cursor_linepos(), ws.ws_col); // DEBUG PRINT
	if (data->index < data->len_cur)
	{
		if (get_cursor_linepos() == ws.ws_col)
		{
			ft_putstr("\e[B");
			ft_printf("\e[%iD", ws.ws_col - 1);
		}
		else
			ft_putstr(CURS_RIGHT);
		(data->index)++;
	}
	ft_eprintf("R AFT LINEPOS: %i/%i\n", get_cursor_linepos(), ws.ws_col); // DEBUG PRINT
}