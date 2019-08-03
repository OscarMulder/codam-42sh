/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   input_clear_buffer.c                               :+:    :+:            */
/*                                                     +:+                    */
/*   By: rkuijper <rkuijper@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/08/03 11:29:21 by rkuijper       #+#    #+#                */
/*   Updated: 2019/08/03 16:02:00 by rkuijper      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"

/*
**
**	HEAVY WIP
**	Will be relied on heavily by all functions that remove characters 
**	from the input buffer to allow for multilines to work.
**
*/
void		input_clear_buffer(unsigned index, t_vshdata *vshdata,
    unsigned from, unsigned to)
{
	unsigned i;
	unsigned newline_count;

    if (vshdata->line == NULL || from >= to)
        return ;
    input_move_to_index(&index, from, vshdata);

	i = from;
	newline_count = 0;
	while (i < to)
	{
		if (vshdata->line[i] == '\n')
			newline_count++;
		i++;
	}
}
