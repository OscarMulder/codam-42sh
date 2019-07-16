/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   redir_pipe.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: jbrinksm <jbrinksm@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/07/14 10:37:41 by jbrinksm       #+#    #+#                */
/*   Updated: 2019/07/16 17:41:26 by jbrinksm      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"
#include <unistd.h>
#include <sys/wait.h>

int		handle_pipe_bin(int *currentpipe, int *parentpipe, int pipeside)
{
	if (currentpipe != NULL)
	{
		if (pipeside == START_PIPE)
		{	
			// pipe output of this execution to pipe
			if (dup2(currentpipe[1], STDOUT_FILENO) == -1)
				ft_putendl("PIPE ERROR");
			close(currentpipe[1]);
		}
		else if (pipeside == EXTEND_PIPE)
		{
			// Take previous output as input
			if (dup2(currentpipe[0], STDIN_FILENO) == -1)
				ft_putendl("PIPE ERROR");
			close(currentpipe[0]);
			if (parentpipe != NULL)
			{
				// pipe output of this execution to parent pipe
				if (dup2(parentpipe[1], STDOUT_FILENO) == -1)
					ft_putendl("PIPE ERROR");
				close(parentpipe[1]);
			}
		}
	}
	return (FUNCT_SUCCESS);
}

int		redir_loop_pipes(t_ast *pipenode, t_envlst *envlst, int *exit_code, int *parentpipe)
{
	int		currentpipe[2];
	char	**command;

	if (pipe(currentpipe) == -1)
	{
		ft_putendl("ERROR PIPE redir_loop_pipes");
		exit(-1);
	}
	
	if (pipenode->child != NULL && pipenode->child->type == PIPE)
		redir_loop_pipes(pipenode->child, envlst, exit_code, currentpipe);

	if (pipenode->child != NULL && pipenode->child->type != PIPE)
	{
		// START THE PIPE (only runs once)
		command = create_args(pipenode->child);
		if (command != NULL)
			exec_cmd(command, envlst, exit_code, START_PIPE, currentpipe, parentpipe);
	}

	close(currentpipe[1]);

	if (pipenode->sibling != NULL)
	{
		// ADD ANY PIPE EXTENSION (runs always)
		command = create_args(pipenode->sibling);
		if (command != NULL)
			exec_cmd(command, envlst, exit_code, EXTEND_PIPE, currentpipe, parentpipe);
		
	}

	close(currentpipe[0]);
	return (FUNCT_SUCCESS);
}
