/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   input_parse_end.c                                  :+:    :+:            */
/*                                                     +:+                    */
/*   By: rkuijper <rkuijper@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/05/16 13:39:04 by rkuijper       #+#    #+#                */
/*   Updated: 2019/08/07 21:51:14 by jbrinksm      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"
#include <sys/ioctl.h>

// int	input_parse_end(t_inputdata *data, char **line)
// {
// 	if ((data->input_state == INPUT_BRACE && data->c == 'F') || data->c == '\5')
// 	{
// 		ft_printf("%s", *line + data->index);
// 		data->index = ft_strlen(*line);
// 		data->input_state = INPUT_NONE;
// 		return (FUNCT_SUCCESS);
// 	}
// 	return (FUNCT_FAILURE);
// }

int			curs_go_end(t_inputdata *data, t_vshdata *vshdata)
{
	struct winsize	ws;
	int				linepos;
	size_t			strlen;
	int				down;
	int				x_offset;

	ioctl(STDIN_FILENO, TIOCGWINSZ, &ws);
	linepos = get_cursor_linepos();
	strlen = ft_strlen(vshdata->line);
	down = ((vshdata->prompt_len + strlen) - data->index) / ws.ws_col;
	x_offset = ((vshdata->prompt_len + strlen) % ws.ws_col) - linepos + 1;

	ft_eprintf("down %i lr %i\n", down, x_offset);
	if (down > 0)
		ft_printf("\e[%iB", down);
	if (x_offset > 0)
		ft_printf("\e[%iC", x_offset);
	else if (x_offset < 0)
		ft_printf("\e[%iD", x_offset * -1);
	data->index = strlen;
	return (FUNCT_SUCCESS);
}
