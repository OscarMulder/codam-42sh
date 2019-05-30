/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   exec_builtin.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: omulder <omulder@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/05/29 16:59:41 by omulder        #+#    #+#                */
/*   Updated: 2019/05/30 17:39:34 by omulder       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"

int		exec_builtin(char **args, char ***env)
{
	if (ft_strcmp(args[0], "exit") == 0)
		builtin_exit(args);
	else if (ft_strcmp(args[0], "echo") == 0)
		builtin_echo(args);
	ft_strarrdel(env);
	return (0);
}
