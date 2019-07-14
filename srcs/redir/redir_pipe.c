/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   redir_pipe.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: jbrinksm <jbrinksm@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/07/14 10:37:41 by jbrinksm       #+#    #+#                */
/*   Updated: 2019/07/14 12:24:38 by jbrinksm      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"
#include <unistd.h>
#include <sys/wait.h>

int		redir_pipe(t_ast *pipe_node)
{
	int	pipe_ends_left[2];
	int	pipe_ends_right[2];

	(void)pipe_node;
	int	status;

	if (pipe(pipe_ends_left) != 0)
		return (FUNCT_FAILURE);
	if (pipe(pipe_ends_right) != 0)
		return (FUNCT_FAILURE);
	ft_printf("fds: ll: %i lr: %i rl: %i rr: %i\n", pipe_ends_left[0], pipe_ends_left[1], pipe_ends_right[0], pipe_ends_right[1]);
	

	int	ret;
	ret = fork();
	if (ret == -1)
		return (FUNCT_ERROR);
	if (ret == 0)
	{
		dup2(pipe_ends_left[1], STDOUT_FILENO);
		close(pipe_ends_left[1]);

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
			dup2(pipe_ends_left[0], STDIN_FILENO);
			close(pipe_ends_left[0]);
			read(STDIN_FILENO, str, 100);
			ft_printf("Found this on STDIN: %s\n", str);
			exit(0);
		}
		else
		{
			wait(&status);

			// close(pipe_ends_left[1]);
			// close(pipe_ends_left[0]);
			close(pipe_ends_right[1]);
			close(pipe_ends_right[0]);
		}
	}
	return (FUNCT_SUCCESS);
}
