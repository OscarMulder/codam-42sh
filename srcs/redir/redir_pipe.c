/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   redir_pipe.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: jbrinksm <jbrinksm@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/07/14 10:37:41 by jbrinksm       #+#    #+#                */
/*   Updated: 2019/07/18 12:54:02 by tde-jong      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"
#include <unistd.h>
#include <sys/wait.h>

t_pipes	init_pipestruct(void)
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

int		handle_pipe(t_pipes pipes)
{
	if (pipes.currentpipe[0] != PIPE_UNINIT && pipes.currentpipe[1] != PIPE_UNINIT)
	{
		if (pipes.pipeside == PIPE_START)
		{	
			// pipe output of this execution to pipe
			if (dup2(pipes.currentpipe[1], STDOUT_FILENO) == -1)
				ft_putendl("PIPE ERROR");
			close(pipes.currentpipe[1]);
		}
		else if (pipes.pipeside == PIPE_EXTEND)
		{
			// Take previous output as input
			if (dup2(pipes.currentpipe[0], STDIN_FILENO) == -1)
				ft_putendl("PIPE ERROR");
			close(pipes.currentpipe[0]);
			if (pipes.parentpipe[0] != PIPE_UNINIT && pipes.parentpipe[1] != PIPE_UNINIT)
			{
				// pipe output of this execution to parent pipe
				if (dup2(pipes.parentpipe[1], STDOUT_FILENO) == -1)
					ft_putendl("PIPE ERROR");
				close(pipes.parentpipe[1]);
			}
		}
	}
	return (FUNCT_SUCCESS);
}

int		redir_loop_pipes(t_ast *pipenode, t_envlst *envlst, int *exit_code, t_pipes pipes)
{
	char	**command;
	t_pipes	childpipes;

	if (pipe(pipes.currentpipe) == -1)
	{
		ft_putendl("ERROR PIPE redir_loop_pipes");
		return (FUNCT_FAILURE);
	}
	if (pipenode->child != NULL && pipenode->child->type == PIPE)
	{
		childpipes = pipes;
		childpipes.parentpipe[0] = pipes.currentpipe[0];
		childpipes.parentpipe[1] = pipes.currentpipe[1];
		redir_loop_pipes(pipenode->child, envlst, exit_code, childpipes);
	}
	if (pipenode->child != NULL && pipenode->child->type != PIPE)
	{
		// START THE PIPE (only runs once)
		command = create_args(pipenode->child);
		if (command != NULL)
		{
			pipes.pipeside = PIPE_START;
			exec_cmd(command, envlst, exit_code, pipes);
		}
	}
	close(pipes.currentpipe[1]);
	if (pipenode->sibling != NULL)
	{
		// ADD ANY PIPE EXTENSION (runs always)
		command = create_args(pipenode->sibling);
		if (command != NULL)
		{
			pipes.pipeside = PIPE_EXTEND;
			exec_cmd(command, envlst, exit_code, pipes);
		}
	}
	close(pipes.currentpipe[0]);
	return (FUNCT_SUCCESS);
}
