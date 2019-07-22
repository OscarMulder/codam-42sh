/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   redir_pipe.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: jbrinksm <jbrinksm@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/07/14 10:37:41 by jbrinksm       #+#    #+#                */
/*   Updated: 2019/07/22 10:26:40 by jbrinksm      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"
#include <unistd.h>
#include <sys/wait.h>

static int	exec_pipe(t_ast *pipenode, t_envlst *envlst,
int *exit_code, t_pipes pipes)
{
	if (pipenode->child != NULL && pipenode->child->type != PIPE)
	{
		pipes.pipeside = PIPE_START;
		if (exec_complete_command(pipenode->child, envlst, exit_code, pipes)
		!= FUNCT_SUCCESS)
			return (FUNCT_ERROR);
	}
	if (close(pipes.currentpipe[1]) == -1)
		return (FUNCT_ERROR);
	if (pipenode->sibling != NULL)
	{
		pipes.pipeside = PIPE_EXTEND;
		if (exec_complete_command(pipenode->sibling, envlst, exit_code, pipes)
		!= FUNCT_SUCCESS)
			return (FUNCT_ERROR);
	}
	if (close(pipes.currentpipe[0]) != FUNCT_SUCCESS)
		return (FUNCT_ERROR);
	return (FUNCT_SUCCESS);
}

t_pipes		init_pipestruct(void)
{
	t_pipes	pipes;

	pipes.fds.stdin = dup(STDIN_FILENO);
	pipes.fds.stdout = dup(STDOUT_FILENO);
	pipes.fds.stderr = dup(STDERR_FILENO);
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

int			redir_handle_pipe(t_pipes pipes, int *exit_code)
{
	dup2(pipes.fds.stdin, STDIN_FILENO);
	if (pipes.currentpipe[0] != PIPE_UNINIT
	&& pipes.currentpipe[1] != PIPE_UNINIT)
	{
		if (pipes.pipeside == PIPE_START)
		{
			if (dup2(pipes.currentpipe[1], STDOUT_FILENO) == -1)
				*exit_code = E_DUP;
			close(pipes.currentpipe[1]);
		}
		else if (pipes.pipeside == PIPE_EXTEND)
		{
			if (dup2(pipes.currentpipe[0], STDIN_FILENO) == -1)
				*exit_code = E_DUP;
			close(pipes.currentpipe[0]);
			if (pipes.parentpipe[0] != PIPE_UNINIT
			&& pipes.parentpipe[1] != PIPE_UNINIT)
			{
				if (dup2(pipes.parentpipe[1], STDOUT_FILENO) == -1)
					*exit_code = E_DUP;
				close(pipes.parentpipe[1]);
			}
		}
	}
	return (FUNCT_SUCCESS);
}

/*
**	Recursively runs commands of the whole pipesequence, and
**	redirects their input and output according to the pipesequence.
**
**	The child of the last pipenode in the pipesequence is the first
**	command in the pipesequence PIPE_START. All other commands will
**	be siblings of pipenodes, and will thus be PIPE_EXTEND.
*/

int			redir_run_pipesequence(t_ast *pipenode, t_envlst *envlst,
int *exit_code, t_pipes pipes)
{
	t_pipes	childpipes;

	if (pipe(pipes.currentpipe) == -1)
	{
		ft_putendl("vsh: unable to create pipe");
		return (FUNCT_FAILURE);
	}
	if (pipenode->child != NULL && pipenode->child->type == PIPE)
	{
		childpipes = pipes;
		childpipes.parentpipe[0] = pipes.currentpipe[0];
		childpipes.parentpipe[1] = pipes.currentpipe[1];
		redir_run_pipesequence(pipenode->child, envlst, exit_code, childpipes);
	}
	return (FUNCT_SUCCESS);
}
