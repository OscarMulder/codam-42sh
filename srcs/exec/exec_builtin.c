/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   exec_builtin.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: omulder <omulder@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/05/29 16:59:41 by omulder        #+#    #+#                */
/*   Updated: 2019/07/16 21:30:07 by jbrinksm      ########   odam.nl         */
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

bool	exec_builtin(char **args, t_envlst *envlst, int *exit_code, int pipeside, int *currentpipe, int *parentpipe, t_stdfds fds)
{
	(void)envlst;
	
	if (ft_strequ(args[0], "echo"))
	{
		dup2(fds.stdin, STDIN_FILENO);
		handle_pipe(currentpipe, parentpipe, pipeside);
		builtin_echo(args, exit_code);
		dup2(fds.stdout, STDOUT_FILENO);
		dup2(fds.stdin, STDIN_FILENO);
	}
	else if (ft_strequ(args[0], "exit"))
	{
		handle_pipe(currentpipe, parentpipe, pipeside);
		builtin_exit(args, exit_code);
	}
	else
		return (false);
	return (true);
}
