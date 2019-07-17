/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   exec_start.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: omulder <omulder@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/05/29 17:52:22 by omulder        #+#    #+#                */
/*   Updated: 2019/07/17 16:14:16 by jbrinksm      ########   odam.nl         */
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

static char	**create_args(t_ast *ast)
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

static void	redir_input(t_ast *node, int *exit_code)
{
	t_ast	*right;

	right = node->sibling;
	if (node->type == SLESS)
	{
		
	}
}

static void	redir_output(t_ast *node, int *exit_code)
{
	t_ast	*right;

	right = node->sibling;
}

/*
**	This will edit the I/O table based on the redirect given as input.
**	DOESNT WORK YET
*/

static void exec_redir(t_ast *node, t_envlst *envlst, int *exit_code)
{
	t_ast	*probe;
	t_ast	*left;

	(void)exit_code;
	(void)envlst;

	#ifdef DEBUG
	char	*leftstr;
	char	*rightstr;
	if (node->sibling->type == WORD)
		leftstr = node->sibling->value;
	else
		leftstr = parser_return_token_str(node->sibling);
	if (node->child->type == WORD)
		rightstr = node->child->value;
	else
		rightstr = parser_return_token_str(node->child);
	ft_printf("Redirect: %s %s %s\n", leftstr,
		parser_return_token_str(node->type), rightstr);
	#endif

	left = node->sibling;
	if (left->type == IO_NUMBER || left->type == WORD)
	{
		if (node->type == SLESS || node->type == DLESS || node->type == LESSAND)
		{
			redir_input(node, exit_code);
		}
		else if (node->type == SGREAT || node->type == DGREAT
			|| node->type == GREATAND)
		{
			redir_output(node, exit_code);
		}
	}
}

static void	exec_assign(t_ast *node, t_envlst *envlst, int *exit_code)
{
	builtin_assign(node->value, envlst, exit_code);
}

/*
**	This is used to handle all the redirects and/or assignments in a
**	complete_command
*/

static void	exec_redirs_or_assigns(t_ast *node, t_envlst *envlst, int *exit_code)
{
	t_ast	*probe;

	probe = node;
	while (probe != NULL)
	{
		if (tool_is_redirect_tk(node->type) == true)
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

static void	exec_complete_command(t_ast *node, t_envlst *envlst, int *exit_code, int flags)
{
	char	**command;

	(void)flags;

	/* Replace wildcards */
	/* Replace variables */

	/* There is atleast one cmd_word in complete_command */
	exec_quote_remove(node);
	if (node->type == WORD)
	{
		if (node->sibling)
			exec_redirs_or_assigns(node->sibling, envlst, exit_code);

		/* Remove useless quotes */
		/* Remove useless escape chars */
		
		command = create_args(node);
		/* add handling of flag = EXEC_PIPE */
		/* add option for flag = EXEC_BG */
		if (command != NULL)
			exec_cmd(command, envlst, exit_code);
	}

	/* There is no cmd_word in complete_command */
	else if (node->type == ASSIGN || tool_is_redirect_tk(node->type) == true)
		exec_redirs_or_assigns(node, envlst, exit_code);
}

/*
**	General structure:
**	Read PR.
*/

void		exec_start(t_ast *ast, t_envlst *envlst, int *exit_code, int flags)
{
	if (ast == NULL)
		return ;
	/* Set flags */
	if (ast->type == PIPE)
		flags &= ~EXEC_PIPE;
	else if (ast->type == BG)
		flags &= ~EXEC_BG;
	else if (ast->type == AND_IF)
		flags &= ~EXEC_AND_IF;
	else if (ast->type == OR_IF)
		flags &= ~EXEC_OR_IF;
	else if (ast->type == SEMICOL)
		flags &= ~EXEC_SEMICOL;

	/* Goes through the tree to find complete_commands first */
	/* problem if there are no WORD's but only prefix or suffix */
	if (ast->type != WORD && ast->type != ASSIGN && ast->type != SGREAT )
		exec_start(ast->child, envlst, exit_code, flags);
	
	/* Runs after the above exec_start returns or isn't run */
	if (ast->type == AND_IF && *exit_code != EXIT_SUCCESS)
		return ;
	else if (ast->type == WORD || tool_is_redirect_tk(ast->type) == true)
		exec_complete_command(ast, envlst, exit_code, flags);
	else if (ast->sibling != NULL)
		exec_start(ast->sibling, envlst, exit_code, flags);
}
