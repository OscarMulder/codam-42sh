/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   exec_cmd.c                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: omulder <omulder@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/05/29 17:17:48 by omulder        #+#    #+#                */
/*   Updated: 2019/06/01 12:34:11 by omulder       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

static int		clean_return(char ***args, int ret)
{
	ft_strarrdel(args);
	return (ret);
}

int		exec_cmd(char **args, char ***env)
{
	int ret;
	int	status;

	ret = exec_builtin(args, env);
	if (ret == 1 || ret == -1)
		return (clean_return(&args, FUNCT_SUCCESS));
	ret = exec_extern(args, *env, &status);
	return (clean_return(&args, FUNCT_SUCCESS));
}
