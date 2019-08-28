/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   input_parse_end.c                                  :+:    :+:            */
/*                                                     +:+                    */
/*   By: rkuijper <rkuijper@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/05/16 13:39:04 by rkuijper       #+#    #+#                */
/*   Updated: 2019/08/28 16:52:04 by tde-jong      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"
#include <sys/ioctl.h>

int			curs_go_end(t_vshdata *data)
{
	size_t	index_to_end;

	index_to_end = data->line->len_cur - data->line->index;
	if (index_to_end > 0)
		curs_move_n_right(data, index_to_end);
	return (FUNCT_SUCCESS);
}
