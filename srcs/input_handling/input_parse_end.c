/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   input_parse_end.c                                  :+:    :+:            */
/*                                                     +:+                    */
/*   By: rkuijper <rkuijper@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/05/16 13:39:04 by rkuijper       #+#    #+#                */
/*   Updated: 2019/08/28 16:54:28 by tde-jong      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"
#include <sys/ioctl.h>

/*
**	Calculates steps to end and then uses curs_move_n_right.
*/

int			curs_go_end(t_vshdata *data)
{
	curs_move_n_right(data, data->line->len_cur - data->line->index);
	return (FUNCT_SUCCESS);
}
