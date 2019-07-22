/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   redir_tools.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: jbrinksm <jbrinksm@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/07/22 09:18:19 by jbrinksm       #+#    #+#                */
/*   Updated: 2019/07/22 09:51:24 by jbrinksm      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

static int	return_error(int ret, int error, int *exit_code, char *opt_str)
{
	if (error == E_BADFD)
		ft_putstr_fd("vsh: bad file descriptor\n", 2);
	if (error == E_DUP)
		ft_putstr_fd("vsh: failed to duplicate file descriptor\n", 2);
	if (error == E_OPEN)
		ft_putstr_fd("vsh: no such file or directory\n", 2);
	if (error == E_AMBRED)
		ft_eprintf("vsh: %s: bad redirect\n", opt_str);
	if (error == E_CLOSE)
		ft_putstr_fd("vsh: failed to close file descriptor\n", 2);
	*exit_code = EXIT_FAILURE;
	return (ret);
}

/*
**	Creates a pipe in which the heredoc can be written to
**	and returns its READ fd.
*/

int			redir_create_heredoc_fd(char *right_side, int *exit_code)
{
	int		pipefds[2];

	*exit_code = EXIT_FAILURE;
	if (pipe(pipefds) == -1)
		return (FUNCT_ERROR);
	if (write(pipefds[1], right_side, ft_strlen(right_side)) == -1)
	{
		close(pipefds[0]);
		close(pipefds[1]);
		return (FUNCT_ERROR);
	}
	close(pipefds[1]);
	*exit_code = EXIT_SUCCESS;
	return (pipefds[0]);
}

/*
**	If node->sibling->child != NULL, it means that the left side
**	of the redirection is explicitly given, and such the right side
**	of the redirection is then 'node->sibling->child' != NULL instead
**	of 'node->sibling'.
*/

void		redir_change_if_leftside(t_ast *node, int *left_side_fd,
char **right_side)
{
	if (node->sibling->child != NULL)
	{
		*left_side_fd = ft_atoi(node->sibling->value);
		*right_side = node->sibling->child->value;
	}
}

int			redir_input_closefd(int left_side_fd, int *exit_code)
{
	if (close(left_side_fd) == -1)
		return (return_error(FUNCT_ERROR, E_CLOSE, exit_code, NULL));
	return (FUNCT_SUCCESS);
}

bool		redir_is_open_fd(int fd)
{
	struct stat buf;

	if (fstat(fd, &buf) == -1)
		return (false);
	return (true);
}
