/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   exec_start.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: omulder <omulder@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/05/29 17:52:22 by omulder        #+#    #+#                */
/*   Updated: 2019/07/23 15:16:54 by jbrinksm      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"
#include <unistd.h>

static size_t	count_args(t_ast *ast)
{
	t_ast	*probe;
	size_t	i;

	i = 0;
	probe = ast;
	while (probe != NULL)
	{
		i++;
		probe = probe->child;
	}
	return (i);
}

char		**create_args(t_ast *ast)
{
	char	**args;
	t_ast	*probe;
	size_t	total_args;
	size_t	i;

	total_args = count_args(ast);
	args = (char**)ft_memalloc(sizeof(char*) * (total_args + 1));
	if (args == NULL)
		return (NULL);
	i = 0;
	probe = ast;
	while (i < total_args)
	{
		args[i] = ft_strdup(probe->value);
		if (args[i] == NULL)
		{
			ft_strarrdel(&args);
			return (NULL);
		}
		probe = probe->child;
		i++;
	}
	return (args);
}

/*
**	This is used to handle all the redirects and/or assignments in a
**	complete_command
*/

static int	exec_redirs_or_assigns(t_ast *node, t_envlst *envlst, int env_type,
int *exit_code)
{
	t_ast	*probe;

	probe = node;
	while (probe != NULL)
	{
		if (tool_is_redirect_tk(node->type) == true)
		{
			if (redir(probe, exit_code) == FUNCT_ERROR)
				return (FUNCT_ERROR);
		}
		else if (probe->type == ASSIGN)
		{
			if (builtin_assign(node->value, envlst, exit_code, env_type)
			== FUNCT_ERROR)
				return (FUNCT_ERROR);
		}
		probe = probe->child;
	}
	return (FUNCT_SUCCESS);
}

static int	redir_save_stdfds(int *stdfds)
{
	STDIN_BAK = dup(STDIN_FILENO);
	if (STDIN_BAK == -1)
		return (FUNCT_ERROR);
	STDOUT_BAK = dup(STDOUT_FILENO);
	if (STDIN_BAK == -1)
		return (FUNCT_ERROR);
	STDERR_BAK = dup(STDERR_FILENO);
	if (STDIN_BAK == -1)
		return (FUNCT_ERROR);
	return (FUNCT_SUCCESS);
}

static int	redir_reset_stdfds(int *stdfds)
{
	if (dup2(STDIN_BAK, STDIN_FILENO) == -1)
		return (FUNCT_ERROR);
	if (dup2(STDOUT_BAK, STDOUT_FILENO) == -1)
		return (FUNCT_ERROR);
	if (dup2(STDERR_BAK, STDERR_FILENO) == -1)
		return (FUNCT_ERROR);
	return (FUNCT_SUCCESS);
}

/*
**	This function has to prepare the complete_command before
**	execution.
*/

int			exec_complete_command(t_ast *node, t_envlst *envlst, int *exit_code, t_pipes pipes)
{
	char	**command;
	int		stdfds[3];

	if (redir_save_stdfds(stdfds) == FUNCT_ERROR)
		return (FUNCT_ERROR);
	exec_quote_remove(node);
	if (node->type == WORD)
	{
		if (node->sibling &&
		exec_redirs_or_assigns(node->sibling, envlst, ENV_TEMP, exit_code)
		== FUNCT_ERROR)
			return (FUNCT_ERROR);
		command = create_args(node);
		if (command != NULL)
			exec_cmd(command, envlst, exit_code, pipes);
	}
	else if (node->type == ASSIGN || tool_is_redirect_tk(node->type) == true)
	{
		if (exec_redirs_or_assigns(node, envlst, ENV_LOCAL, exit_code)
		== FUNCT_ERROR)
			return (FUNCT_ERROR);
	}
	if (redir_reset_stdfds(stdfds) == FUNCT_ERROR)
		return (FUNCT_ERROR);
	return (FUNCT_SUCCESS);
}

/*
**	General structure:
**	Read PR.
*/

int			exec_start(t_ast *ast, t_envlst *envlst, int *exit_code, t_pipes pipes)
{
	if (ast == NULL)
		return (FUNCT_ERROR);
	if (ast->type == PIPE)
		return (redir_run_pipesequence(ast, envlst, exit_code, pipes));
	if (ast->type != WORD && ast->type != ASSIGN)
	{
		if (tool_is_redirect_tk(ast->type) == false)
			exec_start(ast->child, envlst, exit_code, pipes);
	}
	if (ast->type == AND_IF && *exit_code != EXIT_SUCCESS)
		return (FUNCT_ERROR);
	else if (ast->type == OR_IF && *exit_code == EXIT_SUCCESS)
		return (FUNCT_FAILURE);
	else if (ast->type == WORD || tool_is_redirect_tk(ast->type) == true)
	{
		if (exec_complete_command(ast, envlst, exit_code, pipes)
		== FUNCT_SUCCESS)
			return (FUNCT_ERROR);
	}
	else if (ast->sibling != NULL && (ast->sibling->type == WORD
	|| ast->sibling->type == ASSIGN
	|| tool_is_redirect_tk(ast->sibling->type)))
	{
		if (exec_start(ast->sibling, envlst, exit_code, pipes) == FUNCT_ERROR)
			return (FUNCT_ERROR);
	}
	return (FUNCT_SUCCESS);
}
