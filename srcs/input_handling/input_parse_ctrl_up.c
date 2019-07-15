/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   input_parse_ctrl_up.c                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: rkuijper <rkuijper@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/05/16 15:03:17 by rkuijper       #+#    #+#                */
/*   Updated: 2019/07/15 16:44:10 by omulder       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"
#include <sys/ioctl.h>
#include <unistd.h>

static void	parse_ctrl_line_up(unsigned *index)
{
	struct winsize	ws;

	ioctl(STDIN_FILENO, TIOCGWINSZ, &ws);
	if (*index >= ws.ws_col)
	{
		ft_printf("\e[A");
		*index -= ws.ws_col;
	}
	else
	{
		ft_printf("\e[%dD", *index);
		*index = 0;
	}
}

int			input_parse_ctrl_up(t_inputdata *data, t_history **history, char **line)
{
	(void)line;
	if ((data->input_state == INPUT_BRACE || data->input_state == INPUT_D_BRACE) &&
		data->c == 'A')
	{
		if (data->input_state == INPUT_BRACE)
			history_change_line(history, line, &data->index, ARROW_UP);
		else
			parse_ctrl_line_up(&data->index);
		data->input_state = INPUT_NONE;
		return (FUNCT_SUCCESS);
	}
	return (FUNCT_FAILURE);
}
