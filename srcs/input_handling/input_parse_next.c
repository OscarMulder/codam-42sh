/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   input_parse_next.c                                 :+:    :+:            */
/*                                                     +:+                    */
/*   By: omulder <omulder@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/05/16 13:41:00 by rkuijper       #+#    #+#                */
/*   Updated: 2019/08/08 15:08:16 by jbrinksm      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"
#include <sys/ioctl.h>

// static void	parse_next_move_word(unsigned *index, char **line)
// {
// 	unsigned i;

// 	i = *index + 1;
// 	if (*index == ft_strlen(*line))
// 		return ;
// 	while ((*line)[i])
// 	{
// 		if (input_is_word_start(*line, i - 1, i))
// 			break ;
// 		i++;
// 	}
// 	ft_printf("%.*s", i - *index, *line + *index);
// 	*index = i;
// }

// int			input_parse_next(t_inputdata *data, char **line)
// {
// 	if (((data->input_state == INPUT_BRACE ||
// 	data->input_state == INPUT_D_BRACE) && data->c == 'C') ||
// 	(data->input_state == INPUT_ESC && data->c == 'f'))
// 	{
// 		if (data->input_state == INPUT_BRACE)
// 		{
// 			if (data->index < ft_strlen(*line))
// 			{
// 				ft_putchar((*line)[data->index]);
// 				data->index += 1;
// 			}
// 		}
// 		else
// 			parse_next_move_word(&data->index, line);
// 		data->input_state = INPUT_NONE;
// 		return (FUNCT_SUCCESS);
// 	}
// 	return (FUNCT_FAILURE);
// }

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
	struct winsize	ws;
	size_t			linelen;
	int				linepos;
	int				down;
	int				x_offset;

	linelen = ft_strlen(vshdata->line);
	if (n == 0 || linelen == data->index)
		return ;
	if (n > linelen - data->index)
		n = linelen - data->index;
	ioctl(STDIN_FILENO, TIOCGWINSZ, &ws);
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
	struct winsize	ws;
	size_t			linelen;

	ioctl(STDIN_FILENO, TIOCGWINSZ, &ws);
	linelen = ft_strlen(line);
	ft_eprintf("R BEF LINEPOS: %i/%i\n", get_cursor_linepos(), ws.ws_col);
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
	ft_eprintf("R AFT LINEPOS: %i/%i\n", get_cursor_linepos(), ws.ws_col);
}