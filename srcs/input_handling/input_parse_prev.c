/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   input_parse_prev.c                                 :+:    :+:            */
/*                                                     +:+                    */
/*   By: omulder <omulder@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/05/16 13:39:59 by rkuijper       #+#    #+#                */
/*   Updated: 2019/08/08 15:23:19 by jbrinksm      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"
#include <sys/ioctl.h>

// static void	parse_prev_move_word(unsigned *index, char **line)
// {
// 	unsigned i;

// 	i = *index;
// 	if (*index == 0)
// 		return ;
// 	while (i > 0)
// 	{
// 		if (input_is_word_start(*line, i - 1, i) && i != *index)
// 			break ;
// 		i--;
// 	}
// 	ft_printf("\e[%dD", *index - i);
// 	*index = i;
// }

// int			input_parse_prev(t_inputdata *data, char **line)
// {
// 	if (((data->input_state == INPUT_BRACE ||
// 	data->input_state == INPUT_D_BRACE) && data->c == 'D') ||
// 	(data->input_state == INPUT_ESC && data->c == 'b'))
// 	{
// 		if (data->input_state == 2)
// 		{
// 			if (data->index > 0)
// 			{
// 				ft_printf("\e[D");
// 				data->index -= 1;
// 			}
// 		}
// 		else
// 			parse_prev_move_word(&data->index, line);
// 		data->input_state = INPUT_NONE;
// 		return (FUNCT_SUCCESS);
// 	}
// 	return (FUNCT_FAILURE);
// }

void		curs_move_prev_word(t_inputdata *data, t_vshdata *vshdata)
{
	int	i;

	if (data->index == 0)
		return ;
	i = 0;
	if ((data->index > 0
		&& tools_isprintnotblank(vshdata->line[data->index]) == true // i++ if at beginning of previous word
		&& ft_isblank(vshdata->line[data->index - 1]) == true)
		|| (data->index > 0 && data->index == ft_strlen(vshdata->line))) // i++ if at end of line
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

void		curs_move_n_left(t_inputdata *data, size_t n)
{
	struct winsize	ws;
	int				linepos;
	int				up;
	int				x_offset;

	ioctl(STDIN_FILENO, TIOCGWINSZ, &ws);
	linepos = get_cursor_linepos();
	if (n == 0)
		return ;
	if (n > data->index)
		n = data->index;
	up = ((ws.ws_col - linepos) + n) / ws.ws_col;
	x_offset = (ws.ws_col - linepos) - (((ws.ws_col - linepos) + n) % ws.ws_col); // lmfao
	if (up > 0)
		ft_printf("\e[%iA", up);
	if (x_offset > 0)
		ft_printf("\e[%iC", x_offset);
	else if (x_offset < 0)
		ft_printf("\e[%iD", x_offset * -1);
	data->index -= n;
}

void		curs_move_left(t_inputdata *data) //PROTECT
{
	struct winsize	ws;

	ioctl(STDIN_FILENO, TIOCGWINSZ, &ws);
	ft_eprintf("L BEF LINEPOS: %i/%i\n", get_cursor_linepos(), ws.ws_col);
	if (data->index > 0)
	{
		if (get_cursor_linepos() == 1)
		{
			ft_putstr("\e[A");
			ft_printf("\e[%iC", ws.ws_col - 1);
		}
		else
			ft_putstr(CURS_LEFT);
		(data->index)--;
	}
	ft_eprintf("L AFT LINEPOS: %i/%i\n", get_cursor_linepos(), ws.ws_col);
}
