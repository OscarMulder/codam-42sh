/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   redir.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: jbrinksm <jbrinksm@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/07/21 15:14:08 by jbrinksm       #+#    #+#                */
/*   Updated: 2019/07/21 15:51:07 by jbrinksm      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"
#include <unistd.h>
#include <fcntl.h>

static int	get_dless_pipe_fd(char *right_side, int *exit_code)
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
**	If node->sibling->child != NULL, it means that there was a left side
**	specified in the redirect, if that is the case, right_side
**	and left_side_fd will have a different value.
*/

static void	change_if_leftside(t_ast *node, int *left_side_fd, char **right_side)
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
	(void)exit_code;
	char	*right_side;
	int		left_side_fd;
	int		right_side_fd;

	left_side_fd = STDIN_FILENO;
	right_side = node->sibling->value;
	change_if_leftside(node, &left_side_fd, &right_side);
	if (node->type == SLESS)
		right_side_fd = open(right_side, O_RDONLY);
	else if (node->type == DLESS)
		right_side_fd = get_dless_pipe_fd(right_side, exit_code);
	else
		right_side_fd = ft_atoi(right_side);
	if (right_side_fd == -1)
		return (return_error(FUNCT_ERROR));
	dup2(right_side_fd, left_side_fd);
	close(right_side_fd);
	return (FUNCT_SUCCESS);
}

/*
**	Handles `>`, `>>`, and `>&`
*/

int		redir_output(t_ast *node, int *exit_code)
{
	(void)exit_code;
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
	else
		right_side_fd = ft_atoi(right_side);
	if (right_side_fd == -1 || dup2(right_side_fd, left_side_fd) == -1)
	{
		return (FUNCT_ERROR);
	}
	close(right_side_fd);
}

/*
**	Changes file table based on the redirect given as input.
*/

int		redir(t_ast *node, int *exit_code)
{
	if (node == NULL || tool_is_redirect_tk(node->type) == false)
		return (FUNCT_FAILURE);
	if (node->type == SLESS || node->type == DLESS || node->type == LESSAND)
		redir_input(node, exit_code);
	if (node->type == SGREAT || node->type == DGREAT || node->type == GREATAND)
		redir_output(node, exit_code);
	}
}
