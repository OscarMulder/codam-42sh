/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   exec_extern.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: omulder <omulder@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/06/01 11:50:51 by omulder        #+#    #+#                */
/*   Updated: 2019/06/01 12:32:22 by omulder       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

static bool	exec_bin(char **args, char **env, int *status)
{
	pid_t	pid;
	pid_t	wpid;

	pid = fork();
	if (pid < 0)
		return (FUNCT_FAILURE);
	if (pid == 0)
		execve(args[0], args, env);
	wpid = waitpid(pid, status, WUNTRACED);
	if (WIFSIGNALED(*status))
		return (FUNCT_FAILURE);
	return (FUNCT_SUCCESS);
}

bool		exec_extern(char **args, char **env, int *status)
{
	if (args[0][0] != '/')
		return (FUNCT_FAILURE);
	return (exec_bin(args, env, status));
}
