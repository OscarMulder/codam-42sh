/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   curs_move_n_left_hasnewlines.c                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: jbrinksm <jbrinksm@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/08/30 11:34:28 by jbrinksm       #+#    #+#                */
/*   Updated: 2019/08/30 11:34:32 by jbrinksm      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"

static void	move_left_parse_newline(t_vshdata *data)
{
	char	*pos;
	int		len;

	pos = ft_strrnchr(data->line->line, '\n', data->line->index);
	len = data->line->index;
	if (pos != NULL)
		len = (data->line->index - 1) - (pos - data->line->line);
	ft_putstr("\e[A");
	if (len > 1)
		ft_printf("\e[%iC", len);
	data->curs->coords.x = len == 1 ? len : len + 1;
	data->curs->coords.y--;
	if (data->curs->coords.y == 1)
	{
		ft_printf("\e[%iC", data->prompt->prompt_len);
		data->curs->coords.x += data->prompt->prompt_len;
	}
}

static void	move_left_to_colmax(t_vshdata *data, int colmax)
{
	if (data->curs->coords.x == 1)
	{
		data->curs->coords.x = colmax;
		data->curs->coords.y--;
		ft_printf("\e[A\e[%iC", data->curs->coords.x);
	}
	else
	{
		ft_putstr("\e[D");
		data->curs->coords.x--;
	}
}

void		curs_move_n_left_hasnewlines(t_vshdata *data, size_t n)
{
	while (n > 0)
	{
		n--;
		data->line->index--;
		if (data->line->line[data->line->index] == '\n')
			move_left_parse_newline(data);
		else
			move_left_to_colmax(data, data->curs->cur_ws_col);
	}
}
