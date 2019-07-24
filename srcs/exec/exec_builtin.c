/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   exec_builtin.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: omulder <omulder@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/05/29 16:59:41 by omulder        #+#    #+#                */
/*   Updated: 2019/07/24 21:48:24 by mavan-he      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"

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

bool	exec_builtin(char **args, t_envlst *envlst, int *exit_code)
{
	static t_aliaslst *aliaslst;

	if (ft_strequ(args[0], "echo"))
		builtin_echo(args, exit_code);
	else if (ft_strequ(args[0], "exit"))
		builtin_exit(args, exit_code);
	else if (ft_strequ(args[0], "export"))
		builtin_export(args, envlst, exit_code);
	else if (ft_strequ(args[0], "set"))
		builtin_set(args, envlst, exit_code);
	else if (ft_strequ(args[0], "unset"))
		builtin_unset(args, envlst, exit_code);
	else if (ft_strequ(args[0], "alias"))
		builtin_alias(args, &aliaslst);
	else if (ft_strequ(args[0], "unalias"))
		builtin_unalias(args, &aliaslst);
	else
		return (false);
	return (true);
}
