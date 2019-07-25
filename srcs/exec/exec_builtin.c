/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   exec_builtin.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: omulder <omulder@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/05/29 16:59:41 by omulder        #+#    #+#                */
/*   Updated: 2019/07/25 13:26:30 by mavan-he      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"

bool	exec_builtin(char **args, t_envlst *envlst)
{
	static t_aliaslst *aliaslst;

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
	else if (ft_strequ(args[0], "alias"))
		builtin_alias(args, &aliaslst);
	else if (ft_strequ(args[0], "unalias"))
		builtin_unalias(args, &aliaslst);
	else
		return (false);
	return (true);
}
