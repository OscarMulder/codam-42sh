/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   exec_start.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: omulder <omulder@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/05/29 17:52:22 by omulder        #+#    #+#                */
/*   Updated: 2019/07/05 14:47:29 by jbrinksm      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"

static size_t	count_args(t_ast *ast)
{
	t_ast	*probe;
	size_t 	i;

	i = 0;
	probe = ast;
	while (probe != NULL)
	{
		i++;
		probe = probe->child;
	}
	return (i);
}

static void	init_strarr(char **array, size_t size)
{
	size_t i;

	i = 0;
	while (i < size)
	{
		array[i] = NULL;
		i++;
	}
}

static char	**create_args(t_ast *ast)
{
	char	**args;
	t_ast	*probe;
	size_t	total_args;
	size_t	i;

	total_args = count_args(ast);
	args = (char**)ft_memalloc(sizeof(char*) * total_args + 1);
	if (args == NULL)
		return (NULL);
	init_strarr(args, total_args + 1);
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

static void exec_redir(t_ast *node, t_envlst *envlst, int *exit_code)
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
	probe = probe->child;
	if (probe->type == WORD)
		right = probe->value;
	else
		right = parser_return_token_str(probe->type);
	ft_printf("Redirect: %s > %s\n", left, right);
}

static void exec_assign(t_ast *node, t_envlst *envlst, int *exit_code)
{
	(void)envlst;
	(void)exit_code;

	ft_printf("Assignment: %s\n", node->value);
}

static void exec_redirs_or_assigns(t_ast *node, t_envlst *envlst, int *exit_code)
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

static void	exec_tempruntest(t_ast *node, t_envlst *envlst, int *exit_code)
{
	char	**command;

	if (node->sibling)
		exec_redirs_or_assigns(node->sibling, envlst, exit_code);

	command = create_args(node);
	if (command != NULL)
		exec_cmd(command, envlst, exit_code);
}

void		exec_start(t_ast *ast, t_envlst *envlst, int *exit_code)
{
	if (ast->type != WORD && ast->child != NULL)
		exec_start(ast->child, envlst, exit_code);
	if (ast->type == PIPE)
		ft_putendl("Pipe previous output into next command");
	else if (ast->type == BG)
		ft_putendl("Run previous process in the background");
	if (ast->type == AND_IF && *exit_code != EXIT_SUCCESS)
		return ;
	else if (ast->type == WORD)
		exec_tempruntest(ast, envlst, exit_code);
	else
		exec_start(ast->sibling, envlst, exit_code);
}
