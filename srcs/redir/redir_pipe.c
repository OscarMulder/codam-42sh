/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   redir_pipe.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: jbrinksm <jbrinksm@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/07/14 10:37:41 by jbrinksm       #+#    #+#                */
/*   Updated: 2019/07/16 16:51:17 by jbrinksm      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"
#include <unistd.h>
#include <sys/wait.h>

int		handle_pipe_bin(int *pipefdshere, int *pipefdsprev, int pipeside)
{
	if (pipefdshere != NULL)
	{
		if (pipeside == START_PIPE)
		{	
			// pipe output of this execution to pipe
			if (dup2(pipefdshere[1], STDOUT_FILENO) == -1)
				ft_putendl("PIPE ERROR");
			close(pipefdshere[1]);
		}
		else if (pipeside == EXTEND_PIPE)
		{
			// Take previous output as input
			if (dup2(pipefdshere[0], STDIN_FILENO) == -1)
				ft_putendl("PIPE ERROR");
			close(pipefdshere[0]);
			if (pipefdsprev != NULL)
			{
				// pipe output of this execution to parent pipe
				if (dup2(pipefdsprev[1], STDOUT_FILENO) == -1)
					ft_putendl("PIPE ERROR");
				close(pipefdsprev[1]);
			}
		}
	}
	return (FUNCT_SUCCESS);
}

int		redir_loop_pipes(t_ast *pipenode, t_envlst *envlst, int *exit_code, int *pipefdsprev)
{
	int		pipefdshere[2];
	char	**command;

	if (pipe(pipefdshere) == -1)
	{
		ft_putendl("ERROR PIPE redir_loop_pipes");
		exit(-1);
	}
	
	if (pipenode->child != NULL && pipenode->child->type == PIPE)
		redir_loop_pipes(pipenode->child, envlst, exit_code, pipefdshere);

	if (pipenode->child != NULL && pipenode->child->type != PIPE)
	{
		// START THE PIPE (only runs once)
		command = create_args(pipenode->child);
		if (command != NULL)
			exec_cmd(command, envlst, exit_code, START_PIPE, pipefdshere, pipefdsprev);
	}

	close(pipefdshere[1]);

	if (pipenode->sibling != NULL)
	{
		// ADD ANY PIPE EXTENSION (runs always)
		command = create_args(pipenode->sibling);
		if (command != NULL)
			exec_cmd(command, envlst, exit_code, EXTEND_PIPE, pipefdshere, pipefdsprev);
		
	}

	close(pipefdshere[0]);
	return (FUNCT_SUCCESS);
}
