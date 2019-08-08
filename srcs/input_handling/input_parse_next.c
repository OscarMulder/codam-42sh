/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   input_parse_next.c                                 :+:    :+:            */
/*                                                     +:+                    */
/*   By: omulder <omulder@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/05/16 13:41:00 by rkuijper       #+#    #+#                */
/*   Updated: 2019/08/08 16:04:54 by jbrinksm      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"
#include <sys/ioctl.h>

void		curs_move_next_word(t_inputdata *data, t_vshdata *vshdata)
{
	size_t	i;

	if (data->index == ft_strlen(vshdata->line))
		return ;
	i = 0;
	while (ft_isprint(vshdata->line[data->index + i]) == true
		&& ft_isblank(vshdata->line[data->index + i]) == false)
		i++;
	while (ft_isblank(vshdata->line[data->index + i]) == true)
		i++;
	if ((data->index + i == ft_strlen(vshdata->line)) // end of line
		|| (ft_isprint(vshdata->line[data->index + i]) == true
		&& ft_isblank(vshdata->line[data->index + i]) == false))
		curs_move_n_right(data, vshdata, i);
}

void		curs_move_n_right(t_inputdata *data, t_vshdata *vshdata, size_t n)
{
	struct winsize	ws; //WILL BE OSCARS DATA
	size_t			linelen;
	int				linepos;
	int				down;
	int				x_offset;

	linelen = ft_strlen(vshdata->line);
	if (n == 0 || linelen == data->index)
		return ;
	if (n > linelen - data->index)
		n = linelen - data->index;
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

void		curs_move_right(t_inputdata *data, char *line)
{
	struct winsize	ws; //WILL BE OSCARS DATA
	size_t			linelen;

	ioctl(STDIN_FILENO, TIOCGWINSZ, &ws); //WILL BE OSCARS DATA
	linelen = ft_strlen(line);
	ft_eprintf("R BEF LINEPOS: %i/%i\n", get_cursor_linepos(), ws.ws_col); // DEBUG PRINT
	if (data->index < linelen)
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