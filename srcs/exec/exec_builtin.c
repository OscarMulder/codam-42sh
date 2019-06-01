/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   exec_builtin.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: omulder <omulder@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/05/29 16:59:41 by omulder        #+#    #+#                */
/*   Updated: 2019/06/01 10:27:53 by omulder       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"

int		exec_builtin(char **args, char ***env)
{
	if (ft_strcmp(args[0], "exit") == 0)
		builtin_exit(args);
	if (ft_strequ(args[0], "cd") == 0)
		builtin_cd(args, env);
	ft_strarrdel(env);
	return (0);
}
