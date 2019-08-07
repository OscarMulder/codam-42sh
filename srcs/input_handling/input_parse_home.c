/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   input_parse_home.c                                 :+:    :+:            */
/*                                                     +:+                    */
/*   By: omulder <omulder@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/05/16 13:37:33 by rkuijper       #+#    #+#                */
/*   Updated: 2019/08/07 21:48:34 by jbrinksm      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"
#include <sys/ioctl.h>

/*
** The nested if condition is to prevent the cursor to go over the 'prompt' part
** (vsh >) of the terminal line, while also preventing other input functions
** from executing the keypress.
*/

// int	input_parse_home(t_inputdata *data)
// {
// 	if ((data->input_state == INPUT_BRACE && data->c == 'H') || data->c == '\1')
// 	{
// 		if (data->index > 0)
// 			ft_printf("\e[%dD", data->index);
// 		data->index = 0;
// 		data->input_state = INPUT_NONE;
// 		return (FUNCT_SUCCESS);
// 	}
// 	return (FUNCT_FAILURE);
// }

int			curs_go_home(t_inputdata *data, t_vshdata *vshdata)
{
	struct winsize	ws;
	int				linepos;
	int				up;
	int				x_offset;

	ioctl(STDIN_FILENO, TIOCGWINSZ, &ws);
	linepos = get_cursor_linepos();
	up = (vshdata->prompt_len + data->index) / ws.ws_col;
	x_offset = vshdata->prompt_len - linepos + 1;

	ft_eprintf("up %i lr %i\n", up, x_offset);

	if (up > 0)
		ft_printf("\e[%iA", up);
	if (x_offset > 0)
		ft_printf("\e[%iC", x_offset);
	else if (x_offset < 0)
		ft_printf("\e[%iD", x_offset * -1);
	data->index = 0;
	return (FUNCT_SUCCESS);
}
