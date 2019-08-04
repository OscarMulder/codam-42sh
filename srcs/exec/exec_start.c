/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   exec_start.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: omulder <omulder@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/05/29 17:52:22 by omulder        #+#    #+#                */
/*   Updated: 2019/08/04 16:17:34 by jbrinksm      ########   odam.nl         */
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

static char		**create_args(t_ast *ast)
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

static int		exec_redirs_or_assigns(t_ast *node, t_vshdata *vshdata,
	int env_type)
{
	t_ast	*probe;

	probe = node;
	while (probe != NULL)
	{
		if (tool_is_redirect_tk(node->type) == true)
		{
			if (redir(probe) == FUNCT_ERROR)
				return (FUNCT_ERROR);
		}
		else if (probe->type == ASSIGN)
		{
			if (builtin_assign(node->value, vshdata->envlst, env_type)
			== FUNCT_ERROR)
				return (FUNCT_ERROR);
		}
		probe = probe->child;
	}
	return (FUNCT_SUCCESS);
}

int				exec_command(t_ast *ast, t_vshdata *vshdata, t_pipes pipes)
{
	char	**command;

	if (exec_handle_variables(ast, vshdata->envlst) == FUNCT_ERROR)
		return (FUNCT_ERROR);
	exec_quote_remove(ast);
	if (redir_handle_pipe(pipes) == FUNCT_ERROR)
		return (return_and_reset_fds(FUNCT_ERROR, vshdata));
	if (ast->type == WORD)
	{
		if (ast->sibling &&
		exec_redirs_or_assigns(ast->sibling, vshdata, ENV_TEMP) == FUNCT_ERROR)
			return (return_and_reset_fds(FUNCT_ERROR, vshdata));
		command = create_args(ast);
		if (command == NULL)
			return (return_and_reset_fds(FUNCT_ERROR, vshdata));
		exec_cmd(command, vshdata);
	}
	else if (ast->type == ASSIGN || tool_is_redirect_tk(ast->type) == true)
	{
		if (exec_redirs_or_assigns(ast, vshdata, ENV_LOCAL) == FUNCT_ERROR)
			return (return_and_reset_fds(FUNCT_ERROR, vshdata));
	}
	return (return_and_reset_fds(FUNCT_SUCCESS, vshdata));
}

int				exec_pipe_sequence(t_ast *ast, t_vshdata *vshdata, t_pipes pipes)
{
	t_pipes	childpipes;

	/* Skip this phase if there is no PIPE node */
	if (ast->type != PIPE)
		return (exec_command(ast, vshdata, pipes));

	/* create pipe so that childs are properly linked */
	if (pipe(pipes.currentpipe) == -1)
	{
		ft_putendl("vsh: unable to create pipe");
		return (FUNCT_FAILURE);
	}
	/* start with last pipe node */
	if (ast->child->type == PIPE)
	{
		childpipes = pipes;
		childpipes.parentpipe[0] = pipes.currentpipe[0];
		childpipes.parentpipe[1] = pipes.currentpipe[1];
		if (exec_pipe_sequence(ast->child, vshdata, childpipes) == FUNCT_ERROR)
			return (FUNCT_ERROR);
	}

	/* this is the first command node of the pipe sequence */
	if (ast->child->type != PIPE)
	{
		pipes.pipeside = PIPE_START;
		if (exec_command(ast->child, vshdata, pipes) == FUNCT_ERROR)
			return (FUNCT_ERROR);
	}

	/* always attempt to close the read end */
	close(pipes.currentpipe[1]);

	/* these are the other nodes to be piped towards (and from) */
	pipes.pipeside = PIPE_EXTEND;
	if (exec_command(ast->sibling, vshdata, pipes) == FUNCT_ERROR)
		return (FUNCT_ERROR);

	/* always attempt to close the write end */
	close(pipes.currentpipe[0]);
	return (FUNCT_SUCCESS);
}

int				exec_and_or(t_ast *ast, t_vshdata *vshdata)
{
	t_pipes pipes;

	/* init pipes */
	pipes = redir_init_pipestruct();

	/* Skip this phase if no and_or node is present */
	if (ast->type != AND_IF && ast->type != OR_IF)
		return (exec_pipe_sequence(ast, vshdata, pipes));

	/* loop through all and_or nodes */
	if (ast->child->type == AND_IF || ast->child->type == OR_IF)
	{
		if (exec_and_or(ast->child, vshdata) == FUNCT_ERROR)
			return (FUNCT_ERROR);
	}

	if (ast->type == AND_IF && g_state->exit_code != EXIT_SUCCESS)
		return (FUNCT_ERROR);
	else if (ast->type == OR_IF && g_state->exit_code == EXIT_SUCCESS)
		return (FUNCT_FAILURE);

	/* Only runs at the bottom and_or node */
	if (ast->child->type != AND_IF && ast->child->type != OR_IF)
	{
		if (exec_pipe_sequence(ast->child, vshdata, pipes) == FUNCT_ERROR)
			return (FUNCT_ERROR);
	}

	/* Runs once every node */
	if (ast->sibling->type != AND_IF && ast->sibling->type != OR_IF)
	{
		if ((ast->type == AND_IF && g_state->exit_code != EXIT_SUCCESS)
		|| (ast->type == OR_IF && g_state->exit_code == EXIT_SUCCESS))
			return (FUNCT_FAILURE);
		if (exec_pipe_sequence(ast->sibling, vshdata, pipes) == FUNCT_ERROR)
			return (FUNCT_ERROR);
	}
	return (FUNCT_SUCCESS);
}

int				exec_list(t_ast *ast, t_vshdata *vshdata)
{
	/* Skip this phase if node no seperator is present */
	if (ast->type != BG && ast->type != SEMICOL)
		return (exec_and_or(ast, vshdata));

	/* loop through all the seperator nodes */
	if (ast->child->type == BG || ast->child->type == SEMICOL)
	{
		if (exec_list(ast->child, vshdata) == FUNCT_ERROR)
			return (FUNCT_ERROR);
	}

	/* do optional BG shenanigans */

	/* only runs at the bottom node */
	if (ast->child->type != BG && ast->child->type != SEMICOL)
	{
		if (exec_and_or(ast->child, vshdata) == FUNCT_ERROR)
			return (FUNCT_ERROR);
	}

	/* runs once every node */
	if (ast->sibling->type != BG || ast->sibling->type != SEMICOL)
	{
		if (exec_and_or(ast->sibling, vshdata) == FUNCT_ERROR)
			return (FUNCT_ERROR);
	}
	return (FUNCT_SUCCESS);
}

int				exec_complete_command(t_ast *ast, t_vshdata *vshdata)
{
	if (ast == NULL)
		return (FUNCT_ERROR);

	/* run list */
	exec_list(ast, vshdata);

	/* run optional seperator */
	
	return (FUNCT_SUCCESS);
}
