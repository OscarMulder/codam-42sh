/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   input_parse_prev.c                                 :+:    :+:            */
/*                                                     +:+                    */
/*   By: omulder <omulder@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/05/16 13:39:59 by rkuijper       #+#    #+#                */
/*   Updated: 2019/08/15 10:46:05 by rkuijper      ########   odam.nl         */
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
		curs_move_n_left(data, i);
	else
	{
		while (data->index - i > 0
			&& tools_isprintnotblank(vshdata->line[data->index - i - 1]))
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

void		curs_move_n_left(t_inputdata *data, size_t n)
{
	struct winsize	ws; //WILL BE OSCARS DATA
	int				linepos;
	int				up;
	int				x_offset;

	ioctl(STDIN_FILENO, TIOCGWINSZ, &ws); //WILL BE OSCARS DATA
	linepos = get_cursor_linepos();
	if (n == 0)
		return ;
	if (n > data->index)
		n = data->index;
	up = ((ws.ws_col - linepos) + n) / ws.ws_col;
	x_offset = (ws.ws_col - linepos) - (((ws.ws_col - linepos) + n) % ws.ws_col);
	if (up > 0)
		ft_printf("\e[%iA", up);
	if (x_offset > 0)
		ft_printf("\e[%iC", x_offset);
	else if (x_offset < 0)
		ft_printf("\e[%iD", x_offset * -1);
	data->index -= n;
}

/*
**	`ws` will be taken from data after Oscar is done.
**
**	Moves the cursor (and index) one to the left (or up if necessary)
**	If used after some weird screen clearing, make sure to compensate
**	for the automatic `index` change if necessar
*/

void		curs_move_left(t_inputdata *data)
{
	struct winsize	ws; //WILL BE OSCARS DATA

	ioctl(STDIN_FILENO, TIOCGWINSZ, &ws); //WILL BE OSCARS DATA
	ft_eprintf("L BEF LINEPOS: %i/%i\n", get_cursor_linepos(), ws.ws_col); // DEBUG PRINT
	if (data->index > 0)
	{
		// Needs to account for newline characters.
		if (get_cursor_linepos() == 1)
		{
			ft_putstr("\e[A");
			ft_printf("\e[%iC", ws.ws_col - 1);
		}
		else
			ft_putstr(CURS_LEFT);
		(data->index)--;
	}
	ft_eprintf("L AFT LINEPOS: %i/%i\n", get_cursor_linepos(), ws.ws_col); // DEBUG PRINT
}
