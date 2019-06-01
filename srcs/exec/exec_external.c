/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   exec_external.c                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: tde-jong <tde-jong@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/05/31 10:47:19 by tde-jong       #+#    #+#                */
/*   Updated: 2019/06/01 13:28:10 by omulder       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"

static bool	exec_bin(char **args, char **env, int *exit_code)
{
	pid_t	pid;

	pid = fork();
	if (pid < 0)
		return (false);
	if (pid == 0)
		execve(args[0], args, env);
	waitpid(pid, exit_code, WUNTRACED);
	if (WIFSIGNALED(*exit_code))
		return (false);
	return (true);
}

bool		exec_external(char **args, char ***env, int *exit_code)
{
	if (args[0][0] != '/')
		return (false);
	return (exec_bin(args, *env, exit_code));
}
