/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   input_parse_prev.c                                 :+:    :+:            */
/*                                                     +:+                    */
/*   By: omulder <omulder@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/05/16 13:39:59 by rkuijper       #+#    #+#                */
/*   Updated: 2019/08/07 20:21:56 by jbrinksm      ########   odam.nl         */
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
