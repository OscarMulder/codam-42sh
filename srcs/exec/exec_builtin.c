/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   exec_builtin.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: omulder <omulder@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/05/29 16:59:41 by omulder        #+#    #+#                */
/*   Updated: 2019/07/23 11:26:09 by tde-jong      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"
#include <unistd.h>

bool	exec_builtin(char **args, t_envlst *envlst, t_pipes pipes)
{
	(void)envlst;

	if (tools_is_builtin(args[0]) == true)
		redir_handle_pipe(pipes);
	if (ft_strequ(args[0], "echo"))
		builtin_echo(args);
	else if (ft_strequ(args[0], "exit"))
		builtin_exit(args);
	else if (ft_strequ(args[0], "export"))
		builtin_export(args, envlst);
	else if (ft_strequ(args[0], "set"))
		builtin_set(args, envlst);
	else if (ft_strequ(args[0], "unset"))
		builtin_unset(args, envlst);
	else
		return (false);
	dup2(pipes.fds.stdout, STDOUT_FILENO);
	dup2(pipes.fds.stdout, STDERR_FILENO);
	dup2(pipes.fds.stdin, STDIN_FILENO);
	return (true);
}
