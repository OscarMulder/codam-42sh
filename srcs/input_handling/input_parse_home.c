/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   input_parse_home.c                                 :+:    :+:            */
/*                                                     +:+                    */
/*   By: omulder <omulder@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/05/16 13:37:33 by rkuijper       #+#    #+#                */
/*   Updated: 2019/08/23 15:06:19 by rkuijper      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"
#include <sys/ioctl.h>

/*
**	Calculates steps to home and then uses curs_move_n_left.
*/

int			curs_go_home(t_inputdata *data, t_vshdata *vshdata)
{
	curs_move_n_left(data, vshdata, data->index);
	return (FUNCT_SUCCESS);
}
