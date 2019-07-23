/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   exec_start.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: omulder <omulder@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/05/29 17:52:22 by omulder        #+#    #+#                */
/*   Updated: 2019/07/23 12:19:31 by jbrinksm      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"

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
		#ifdef DEBUG
		if (probe->type != WORD)
			ft_putendl("Found non-WORD item in child-flow of WORD's (aka I messed something up -Jorn)");
		#endif
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
**	This will edit the I/O table based on the redirect given as input.
**	DOESNT WORK YET
*/

static void	exec_redir(t_ast *node, t_envlst *envlst, int *exit_code)
{
	t_ast	*probe;
	char	*left;
	char	*right;

	(void)exit_code;
	(void)envlst;

	probe = node->sibling;
	if (probe->type == WORD)
		left = probe->value;
	else
		left = parser_return_token_str(probe->type);
	probe = node->sibling->child;
	if (probe->type == WORD)
		right = probe->value;
	else
		right = parser_return_token_str(probe->type);
	ft_printf("Redirect: %s > %s\n", left, right);
}

static void	exec_assign(t_ast *node, t_envlst *envlst, int *exit_code)
{
	builtin_assign(node->value, envlst, exit_code, ENV_TEMP);
}

/*
**	This is used to handle all the redirects and/or assignments in a
**	complete_command
*/

void		exec_redirs_or_assigns(t_ast *node, t_envlst *envlst, int *exit_code)
{
	t_ast	*probe;

	probe = node;
	while (probe != NULL)
	{
		if (probe->type == SGREAT)
			exec_redir(probe, envlst, exit_code);
		else if (probe->type == ASSIGN)
			exec_assign(probe, envlst, exit_code);
		probe = probe->child;
	}
}

/*
**	This function has to prepare the complete_command before
**	execution. Wildcard, quote removal, variables.
*/

int			exec_complete_command(t_ast *node, t_envlst *envlst, int *exit_code, t_pipes pipes)
{
	char	**command;

	exec_quote_remove(node);
	if (node->type == WORD)
	{
		if (node->sibling)
			exec_redirs_or_assigns(node->sibling, envlst, exit_code);
		command = create_args(node);
		if (command == NULL)
			return (FUNCT_ERROR);
		exec_cmd(command, envlst, exit_code, pipes);
	}
	else if (node->type == ASSIGN || tool_is_redirect_tk(node->type) == true)
		exec_redirs_or_assigns(node, envlst, exit_code);
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
	{
		if (redir_run_pipesequence(ast, envlst, exit_code, pipes)
		!= FUNCT_SUCCESS)
			return (FUNCT_ERROR);
		return (FUNCT_SUCCESS);
	}
	/* Goes through the tree to find complete_commands first */
	if ((ast->type != WORD && ast->type != ASSIGN && ast->type != SGREAT)
	&& exec_start(ast->child, envlst, exit_code, pipes) != FUNCT_SUCCESS)
		return (FUNCT_ERROR);
	/* Runs after the above exec_start returns or isn't run */
	if (ast->type == AND_IF && *exit_code != EXIT_SUCCESS)
		return (FUNCT_SUCCESS);
	else if ((ast->type == WORD || ast->type == ASSIGN || ast->type == SGREAT)
	&& exec_complete_command(ast, envlst, exit_code, pipes) != FUNCT_SUCCESS)
		return (FUNCT_ERROR);
	else if (ast->sibling != NULL)
	{
		if (exec_start(ast->sibling, envlst, exit_code, pipes) != FUNCT_SUCCESS)
			return (FUNCT_ERROR);
	}
	return (FUNCT_SUCCESS);
}
