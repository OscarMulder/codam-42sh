/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   redir_pipe.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: jbrinksm <jbrinksm@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/07/14 10:37:41 by jbrinksm       #+#    #+#                */
/*   Updated: 2019/08/04 16:27:24 by jbrinksm      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"
#include <unistd.h>
#include <sys/wait.h>

t_pipes		redir_init_pipestruct(void)
{
	t_pipes	pipes;

	pipes.pipeside = PIPE_UNINIT;
	pipes.parentpipe[0] = PIPE_UNINIT;
	pipes.parentpipe[1] = PIPE_UNINIT;
	pipes.currentpipe[0] = PIPE_UNINIT;
	pipes.currentpipe[1] = PIPE_UNINIT;
	return (pipes);
}

/*
**	If the command which called handle_pipe
**	is the first of the pipe-sequence (PIPE_START),
**	It's output will be redirected to a pipe.
**
**	If the command which called handle_pipe
**	is somewhere inbetween the end and start
**	of the pipe sequence (PIPE EXTEND and with
**	a parentpipe NOT == UNINIT) it will take input
**	from the previous pipe, and send it's output into
**	it's parents pipe.
**
**	If the command which called handle_pipe
**	is at the end of the pipe-sequence (PIPE_EXTEND
**	and it's parentpipe will be UNINIT because it has
**	no parent) only the input will be taken from the
**	previous pipe, and the output will be to STDOUT.
*/

int			redir_handle_pipe(t_pipes pipes)
{
	if (pipes.currentpipe[0] != PIPE_UNINIT
	&& pipes.currentpipe[1] != PIPE_UNINIT)
	{
		if (pipes.pipeside == PIPE_START)
		{
			if (dup2(pipes.currentpipe[1], STDOUT_FILENO) == -1)
				error_return(FUNCT_ERROR, E_DUP, NULL);
			close(pipes.currentpipe[1]);
		}
		else if (pipes.pipeside == PIPE_EXTEND)
		{
			if (dup2(pipes.currentpipe[0], STDIN_FILENO) == -1)
				error_return(FUNCT_ERROR, E_DUP, NULL);
			close(pipes.currentpipe[0]);
			if (pipes.parentpipe[0] != PIPE_UNINIT
			&& pipes.parentpipe[1] != PIPE_UNINIT)
			{
				if (dup2(pipes.parentpipe[1], STDOUT_FILENO) == -1)
					error_return(FUNCT_ERROR, E_DUP, NULL);
				close(pipes.parentpipe[1]);
			}
		}
	}
	return (FUNCT_SUCCESS);
}
