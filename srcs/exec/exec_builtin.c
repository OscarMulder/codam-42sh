/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   exec_builtin.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: omulder <omulder@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/05/29 16:59:41 by omulder        #+#    #+#                */
/*   Updated: 2019/07/16 17:35:20 by jbrinksm      ########   odam.nl         */
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

bool	exec_builtin(char **args, t_envlst *envlst, int *exit_code, int pipeside, int *currentpipe, int *parentpipe)
{
	(void)envlst;
	(void)pipeside;
	(void)currentpipe;
	(void)parentpipe;
	if (ft_strequ(args[0], "echo"))
	{
		// handle_pipe(pipefds, NULL, pipeside);
		builtin_echo(args, exit_code);
	}
	else if (ft_strequ(args[0], "exit"))
	{
		// handle_pipe(pipefds, NULL< pipeside);
		builtin_exit(args, exit_code);
	}
	else
		return (false);
	return (true);
}
