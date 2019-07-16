/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   exec_builtin.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: omulder <omulder@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/05/29 16:59:41 by omulder        #+#    #+#                */
/*   Updated: 2019/07/16 22:37:35 by jbrinksm      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"
#include <unistd.h>

/*
*** Other builtins to be added
**	else if (ft_strequ(args[0], "cd"))
**		builtin_cd(args, exit_code);
**	else if (ft_strequ(args[0], "setenv"))
**		builtin_setenv(args, exit_code);
**	else if (ft_strequ(args[0], "unsetenv"))
**		builtin_unsetenv(args, exit_code);
**	else if (ft_strequ(args[0], "env"))
**		builtin_env(args, exit_code);
*/

bool	exec_builtin(char **args, t_envlst *envlst, int *exit_code, t_pipes pipes)
{
	(void)envlst;
	
	if (ft_strequ(args[0], "echo"))
	{
		dup2(pipes.fds.stdin, STDIN_FILENO);
		handle_pipe(pipes);
		builtin_echo(args, exit_code);
		dup2(pipes.fds.stdout, STDOUT_FILENO);
		dup2(pipes.fds.stdin, STDIN_FILENO);
	}
	else if (ft_strequ(args[0], "exit"))
	{
		handle_pipe(pipes);
		builtin_exit(args, exit_code);
	}
	else
		return (false);
	return (true);
}
