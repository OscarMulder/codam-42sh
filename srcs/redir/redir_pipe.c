/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   redir_pipe.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: jbrinksm <jbrinksm@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/07/14 10:37:41 by jbrinksm       #+#    #+#                */
/*   Updated: 2019/07/15 13:46:46 by jbrinksm      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"
#include <unistd.h>
#include <sys/wait.h>

int		handle_pipe(int *pipefds, int pipeside)
{
	if (pipefds != NULL)
	{
		if (pipeside == LEFT)
		{	
			if (dup2(pipefds[1], STDOUT_FILENO) == -1)
				ft_putendl("PIPE ERROR");
			close(pipefds[1]);
		}
		else if (pipeside == RIGHT)
		{
			if (dup2(pipefds[0], STDIN_FILENO) == -1)
				ft_putendl("PIPE ERROR");
			close(pipefds[0]);
		}
	}
	return (FUNCT_SUCCESS);
}

int		close_pipe(int *pipefds)
{
	if (pipefds != NULL)
	{
		close(pipefds[0]);
		close(pipefds[1]);
	}
	return (FUNCT_SUCCESS);
}

int		redir_add_lstitem(t_piperedirs **lsthead, )

int		redir_loop_pipes(t_ast *pipehead, t_envlst *envlst, int *exit_code)
{
	t_piperedirs	*pipelst;
	t_ast			*probe;

	pipelst = ft_memalloc(sizeof(t_piperedirs));
	pipelst = NULL;
	probe = pipehead;
	while (probe->type == PIPE)
	{
		probe = probe->next;
	}
}

int		redir_pipe_test(t_ast *pipenode, t_envlst *envlst, int *exit_code)
{
	int		pipefds[2];
	char	**command;

	if (pipe(pipefds) != 0)
	{
		ft_putendl("ERROR TRYING TO MAKE PIPE");
		return (FUNCT_ERROR);
	}
	
	exec_quote_remove(pipenode->sibling);
	if (pipenode->child->sibling)
		exec_redirs_or_assigns(pipenode->child->sibling, envlst, exit_code);
	command = create_args(pipenode->child);
	if (command != NULL)
		exec_cmd(command, envlst, exit_code, LEFT, pipefds);

	close(pipefds[1]);

	exec_quote_remove(pipenode->child);
	if (pipenode->sibling->sibling)
		exec_redirs_or_assigns(pipenode->sibling->sibling, envlst, exit_code);
	command = create_args(pipenode->sibling);
	if (command != NULL)
		exec_cmd(command, envlst, exit_code, RIGHT, pipefds);


	close(pipefds[0]);
	return (FUNCT_SUCCESS);
}
