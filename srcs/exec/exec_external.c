/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   exec_external.c                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: tde-jong <tde-jong@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/05/31 10:47:19 by tde-jong       #+#    #+#                */
/*   Updated: 2019/07/16 21:28:40 by jbrinksm      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"
#include "unistd.h"
#include <sys/wait.h>

static bool	exec_bin(char **args, char **vshenviron, int *exit_code, int pipeside, int *currentpipe, int *parentpipe, t_stdfds fds)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid < 0)
		return (false);
	if (pid == 0)
	{
		dup2(fds.stdout, STDIN_FILENO);
		handle_pipe(currentpipe, parentpipe, pipeside);
		execve(args[0], args, vshenviron);
	}
	waitpid(pid, &status, WUNTRACED);
	if (WIFEXITED(status))
		*exit_code = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		*exit_code = EXIT_FATAL + WTERMSIG(status);
	return (true);
}

bool		exec_external(char **args, t_envlst *envlst, int *exit_code, int pipeside, int *currentpipe, int *parentpipe, t_stdfds fds)
{
	char	**vshenviron;
	char	*binary;
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
	ret = exec_bin(args, vshenviron, exit_code, pipeside, currentpipe, parentpipe, fds);
	free(vshenviron);
	return (ret);
}
