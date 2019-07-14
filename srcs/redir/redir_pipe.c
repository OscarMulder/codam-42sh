/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   redir_pipe.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: jbrinksm <jbrinksm@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/07/14 10:37:41 by jbrinksm       #+#    #+#                */
/*   Updated: 2019/07/14 12:43:28 by jbrinksm      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"
#include <unistd.h>
#include <sys/wait.h>

int		redir_pipe(t_ast *pipe_node)
{
	int	pipe_ends[2];

	(void)pipe_node;
	int	status;

	if (pipe(pipe_ends) != 0)
		return (FUNCT_FAILURE);
	ft_printf("fds: R: %i W: %i\n", pipe_ends[0], pipe_ends[1]);
	

	int	ret;
	ret = fork();
	if (ret == -1)
		return (FUNCT_ERROR);
	if (ret == 0)
	{
		dup2(pipe_ends[1], STDOUT_FILENO);
		close(pipe_ends[1]);

		ft_putendl("INPUT");
		exit(0);
	}
	else
	{
		wait(&status);
		int ret2;

		ret2 = fork();
		if (ret2 == -1)
			return (FUNCT_ERROR);
		if (ret2 == 0)
		{
			char	*str = ft_strnew(100);
			dup2(pipe_ends[0], STDIN_FILENO);
			close(pipe_ends[0]);
			read(STDIN_FILENO, str, 100);
			ft_printf("Found this on STDIN: %s\n", str);
			exit(0);
		}
		else
		{
			wait(&status);

			close(pipe_ends[1]);
			close(pipe_ends[0]);
		}
	}
	return (FUNCT_SUCCESS);
}
