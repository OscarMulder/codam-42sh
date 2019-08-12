/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   input_parse_end.c                                  :+:    :+:            */
/*                                                     +:+                    */
/*   By: rkuijper <rkuijper@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/05/16 13:39:04 by rkuijper       #+#    #+#                */
/*   Updated: 2019/08/12 14:30:15 by rkuijper      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"
#include <sys/ioctl.h>


/*
**	Calculates steps to end and then uses curs_move_n_right.
*/

int			curs_go_end(t_inputdata *data, t_vshdata *vshdata)
{
	size_t	index_to_end;

	index_to_end = data->len_cur - data->index;
	if (index_to_end > 0)
		curs_move_n_right(data, vshdata, index_to_end);
	return (FUNCT_SUCCESS);
}
