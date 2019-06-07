/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   exec_external.c                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: tde-jong <tde-jong@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/05/31 10:47:19 by tde-jong       #+#    #+#                */
/*   Updated: 2019/06/06 10:46:47 by tde-jong      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"
#include "unistd.h"
#include <sys/wait.h>

static bool	exec_bin(char **args, char **vshenviron, int *exit_code)
{
	pid_t	pid;
	int		status;
	int		ret;

	pid = fork();
	if (pid < 0)
		return (false);
	if (pid == 0)
	{
		ret = execve(args[0], args, vshenviron);
		if (ret == -1)
		{
			if (args[0][0] == '/')
				ft_eprintf("vsh: %s: no such file or directory\n", args[0]);
			else
				ft_eprintf("vsh: %s: command not found\n", args[0]);
			ft_strarrdel&vshenviron);
			ft_strarrdel&args);
			exit(ret);
		}
	}
	waitpid(pid, &status, WUNTRACED);
	if (WIFEXITED(status))
		*exit_code = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		*exit_code = EXIT_FATAL + WTERMSIG(status);
	return (true);
}

bool		exec_external(char **args, t_envlst *envlst, int *exit_code)
{
	char	**vshenviron;
	char 	*binary;
	bool	ret;

	if (args[0][0] != '/' && !ft_strnequ(args[0], "./", 2))
	{
		binary = exec_find_binary(args[0], envlst);
		if (binary == NULL)
			return (false);
		free(args[0]);
		args[0] = binary;
	}
	vshenviron = env_lsttoarr(envlst, ENV_EXTERN);
	if (vshenviron == NULL)
	{
		ft_printf("vsh: failed to allocate enough memory!\n");
		*exit_code = EXIT_FAILURE;
		return (false);
	}
	ret = exec_bin(args, vshenviron, exit_code);
	free(vshenviron);
	return (ret);
}
