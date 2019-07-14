/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   redir_pipe.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: jbrinksm <jbrinksm@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/07/14 10:37:41 by jbrinksm       #+#    #+#                */
/*   Updated: 2019/07/14 11:11:05 by jbrinksm      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"

int		redir_pipe(t_ast *pipe_node)
{
	int	pipe_ends_left[2];
	int	pipe_ends_right[2];

	if (pipe(pipe_ends_left) != 0)
		return (FUNCT_FAILURE);
	if (pipe(pipe_ends_right) != 0)
		return (FUNCT_FAILURE);
	ft_printf("fds: ll: %i lr: rl: %i rr: %i %i\n", pipe_ends_left[0], pipe_ends_left[1], pipe_ends_right[0], pipe_ends_right[1]);
	return (FUNCT_SUCCESS);
}
