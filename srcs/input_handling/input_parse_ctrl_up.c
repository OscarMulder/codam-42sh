/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   input_parse_ctrl_up.c                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: rkuijper <rkuijper@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/05/16 15:03:17 by rkuijper       #+#    #+#                */
/*   Updated: 2019/05/17 13:37:11 by rkuijper      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"
#include <sys/ioctl.h>

static void		parse_ctrl_line_up(unsigned *index)
{
	struct ttysize		ts;
	
    ioctl(STDIN_FILENO, TIOCGSIZE, &ts);
	if (*index >= ts.ts_cols)
	{
		ft_printf("\e[A");
		(*index) -= ts.ts_cols;
	}
	else
	{
		ft_printf("\e[%dD", *index);
		*index = 0;
	}
}

int				input_parse_ctrl_up(char c, int *input_state,
	unsigned *index, char **line)
{
	(void)line; // Gets used as soon as history is implemented.
	if ((*input_state == 2 || *input_state == 5) && c == 'A')
	{
		if (*input_state == 2) // History.
			;
		else
			parse_ctrl_line_up(index);
		*input_state = 0;
		return (FUNCT_SUCCESS);
	}
	return (FUNCT_FAILURE);
}
