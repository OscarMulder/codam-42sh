/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   redir_pipe.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: jbrinksm <jbrinksm@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/07/14 10:37:41 by jbrinksm       #+#    #+#                */
/*   Updated: 2019/07/16 23:03:33 by jbrinksm      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"
#include <unistd.h>
#include <sys/wait.h>

int		handle_pipe(t_pipes pipes)
{
	if (pipes.currentpipe != NULL)
	{
		if (pipes.pipeside == START_PIPE)
		{	
			// pipe output of this execution to pipe
			if (dup2(pipes.currentpipe[1], STDOUT_FILENO) == -1)
				ft_putendl("PIPE ERROR");
			close(pipes.currentpipe[1]);
		}
		else if (pipes.pipeside == EXTEND_PIPE)
		{
			// Take previous output as input
			if (dup2(pipes.currentpipe[0], STDIN_FILENO) == -1)
				ft_putendl("PIPE ERROR");
			close(pipes.currentpipe[0]);
			if (pipes.parentpipe != NULL)
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

int		redir_loop_pipes(t_ast *pipenode, t_envlst *envlst, int *exit_code, t_pipes *pipes)
{
	char	**command;
	int		*pipeholder;

	if (pipe(pipes->currentpipe) == -1)
	{
		ft_putendl("ERROR PIPE redir_loop_pipes");
		exit(-1);
	}
	
	if (pipenode->child != NULL && pipenode->child->type == PIPE)
	{
		pipeholder = pipes->parentpipe;
		pipes->parentpipe = pipes->currentpipe;
		pipes->currentpipe = pipeholder;
		redir_loop_pipes(pipenode->child, envlst, exit_code, pipes);
	}

	if (pipenode->child != NULL && pipenode->child->type != PIPE)
	{
		// START THE PIPE (only runs once)
		command = create_args(pipenode->child);
		if (command != NULL)
		{
			pipes->pipeside = START_PIPE;
			exec_cmd(command, envlst, exit_code, pipes);
		}
	}

	close(pipes->currentpipe[1]);

	if (pipenode->sibling != NULL)
	{
		// ADD ANY PIPE EXTENSION (runs always)
		command = create_args(pipenode->sibling);
		if (command != NULL)
		{
			pipes->pipeside = EXTEND_PIPE;
			exec_cmd(command, envlst, exit_code, pipes);
		}
	}

	close(pipes->currentpipe[0]);
	return (FUNCT_SUCCESS);
}
