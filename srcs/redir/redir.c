/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   redir.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: jbrinksm <jbrinksm@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/07/21 15:14:08 by jbrinksm       #+#    #+#                */
/*   Updated: 2019/07/21 17:37:25 by jbrinksm      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"
#include <unistd.h>
#include <fcntl.h>

static int	return_error(int ret, int error, int *exit_code)
{
	*exit_code = error;
	return (ret);
}

/*
**	Creates a pipe in which the heredoc can be written to
**	and returns its READ fd.
*/

static int	create_heredoc_fd(char *right_side, int *exit_code)
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

static void	change_if_leftside(t_ast *node, int *left_side_fd,
char **right_side)
{
	if (node->sibling->child != NULL)
	{
		*left_side_fd = ft_atoi(node->sibling->value);
		*right_side = node->sibling->child->value;
	}
}

/*
**	Handles `<`, `<<`, and `<&`
*/

int		redir_input(t_ast *node, int *exit_code)
{
	char	*right_side;
	int		left_side_fd;
	int		right_side_fd;

	left_side_fd = STDIN_FILENO;
	right_side = node->sibling->value;
	change_if_leftside(node, &left_side_fd, &right_side);
	if (node->type == SLESS)
		right_side_fd = open(right_side, O_RDONLY);
	else if (node->type == DLESS)
		right_side_fd = create_heredoc_fd(right_side, exit_code);
	else if (ft_strequ(right_side, "-") == true)
	{
		close(left_side_fd);
		return (FUNCT_SUCCESS);
	}
	else
	{
		right_side_fd = ft_atoi(right_side);
	}
	if (right_side_fd == -1)
		return (return_error(FUNCT_ERROR, E_OPEN, exit_code));
	if (dup2(right_side_fd, left_side_fd) == -1)
		return (return_error(FUNCT_ERROR, E_DUP, exit_code));
	close(right_side_fd);
	return (FUNCT_SUCCESS);
}

/*
**	Handles `>`, `>>`, and `>&`
*/

int		redir_output(t_ast *node, int *exit_code)
{
	char	*right_side;
	int		left_side_fd;
	int		right_side_fd;

	left_side_fd = STDOUT_FILENO;
	right_side = node->sibling->value;
	change_if_leftside(node, &left_side_fd, &right_side);
	if (node->type == SGREAT)
		right_side_fd = open(right_side, O_WRONLY | O_CREAT | O_TRUNC);
	else if (node->type == DGREAT)
		right_side_fd = open(right_side, O_WRONLY | O_CREAT | O_APPEND);
	else if (ft_strequ(right_side, "-") == true)
	{
		close(left_side_fd);
		return (FUNCT_SUCCESS);
	}
	else
	{
		right_side_fd = ft_atoi(right_side);
	}
	if (right_side_fd == -1)
		return (return_error(FUNCT_ERROR, E_OPEN, exit_code));
	if (dup2(right_side_fd, left_side_fd) == -1)
		return (return_error(FUNCT_ERROR, E_DUP, exit_code));
	close(right_side_fd);
	return (FUNCT_SUCCESS);
}

/*
**	Changes file table based on the redirect given as input.
*/

int		redir(t_ast *node, int *exit_code)
{
	int		ret;

	ret = FUNCT_SUCCESS;
	if (node == NULL || tool_is_redirect_tk(node->type) == false)
		return (FUNCT_FAILURE);
	if (node->type == SLESS || node->type == DLESS || node->type == LESSAND)
		ret = redir_input(node, exit_code);
	if (node->type == SGREAT || node->type == DGREAT || node->type == GREATAND)
		ret = redir_output(node, exit_code);
	return (ret);
}
