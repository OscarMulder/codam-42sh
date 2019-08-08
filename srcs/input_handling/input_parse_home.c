/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   input_parse_home.c                                 :+:    :+:            */
/*                                                     +:+                    */
/*   By: omulder <omulder@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/05/16 13:37:33 by rkuijper       #+#    #+#                */
/*   Updated: 2019/08/08 20:01:46 by jbrinksm      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"
#include <sys/ioctl.h>

/*
**	Calculates steps to home and then uses curs_move_n_left.
*/

int			curs_go_home(t_inputdata *data, t_vshdata *vshdata)
{
	size_t	index_to_home;

	(void)vshdata;
	index_to_home = data->index;
	curs_move_n_left(data, index_to_home);
	return (FUNCT_SUCCESS);
}
