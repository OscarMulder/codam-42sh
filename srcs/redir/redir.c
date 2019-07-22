/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   redir.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: jbrinksm <jbrinksm@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/07/21 15:14:08 by jbrinksm       #+#    #+#                */
/*   Updated: 2019/07/22 09:30:10 by jbrinksm      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"
#include <unistd.h>
#include <fcntl.h>

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
	*exit_code = error;
	return (ret);
}

static int	getvalidfd(int *right_side_fd, char *right_side,
int *exit_code)
{
	if (tools_is_fdnumstr(right_side) == false)
		return (return_error(FUNCT_ERROR, E_AMBRED, exit_code, right_side));
	*right_side_fd = ft_atoi(right_side);
	if (redir_is_open_fd(*right_side_fd) == false)
		return (return_error(FUNCT_ERROR, E_BADFD, exit_code, NULL));
	return (FUNCT_SUCCESS);
}

/*
**	Handles `<`, `<<`, and `<&`
*/

int			redir_input(t_ast *node, int *exit_code)
{
	char	*right_side;
	int		left_side_fd;
	int		right_side_fd;

	left_side_fd = STDIN_FILENO;
	right_side = node->sibling->value;
	right_side_fd = FD_UNINIT;
	redir_change_if_leftside(node, &left_side_fd, &right_side);
	if (node->type == SLESS)
		right_side_fd = open(right_side, O_RDONLY);
	else if (node->type == DLESS)
		right_side_fd = redir_create_heredoc_fd(right_side, exit_code);
	else if (ft_strequ(right_side, "-") == true)
		redir_input_closefd(left_side_fd, exit_code);
	else
	{
		if (getvalidfd(&right_side_fd, right_side, exit_code) == FUNCT_ERROR)
			return (FUNCT_ERROR);
	}
	if (right_side_fd == -1)
		return (return_error(FUNCT_ERROR, E_OPEN, exit_code, NULL));
	if (dup2(right_side_fd, left_side_fd) == -1)
		return (return_error(FUNCT_ERROR, E_DUP, exit_code, NULL));
	close(right_side_fd);
	return (FUNCT_SUCCESS);
}

/*
**	Handles `>`, `>>`, and `>&`
*/

int			redir_output(t_ast *node, int *exit_code)
{
	char	*right_side;
	int		left_side_fd;
	int		right_side_fd;

	left_side_fd = STDOUT_FILENO;
	right_side = node->sibling->value;
	right_side_fd = FD_UNINIT;
	redir_change_if_leftside(node, &left_side_fd, &right_side);
	if (node->type == SGREAT)
		right_side_fd = open(right_side, O_WRONLY | O_CREAT | O_TRUNC);
	else if (node->type == DGREAT)
		right_side_fd = open(right_side, O_WRONLY | O_CREAT | O_APPEND);
	else if (ft_strequ(right_side, "-") == true)
		redir_input_closefd(left_side_fd, exit_code);
	else
	{
		if (getvalidfd(&right_side_fd, right_side, exit_code) == FUNCT_ERROR)
			return (FUNCT_ERROR);
	}
	if (right_side_fd == -1)
		return (return_error(FUNCT_ERROR, E_OPEN, exit_code, NULL));
	if (dup2(right_side_fd, left_side_fd) == -1)
		return (return_error(FUNCT_ERROR, E_DUP, exit_code, NULL));
	close(right_side_fd);
	return (FUNCT_SUCCESS);
}

/*
**	Changes file table based on the redirect given as input.
*/

int			redir(t_ast *node, int *exit_code)
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
