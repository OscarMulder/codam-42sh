/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   exec_start.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: omulder <omulder@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/05/29 17:52:22 by omulder        #+#    #+#                */
/*   Updated: 2019/10/28 20:58:03 by jbrinksm      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"

/*
**	Recursively runs commands of the whole pipesequence, and
**	redirects their input and output according to the pipesequence.
**
**	The left of the last pipenode in the pipesequence is the first
**	command in the pipesequence PIPE_START. All other commands will
**	be siblings of pipenodes, and will thus be PIPE_EXTEND.
*/

int				exec_pipe_sequence(t_ast *ast, t_vshdata *data)
{
	if (ast->type != PIPE)
		return (exec_command(ast, data));
	if (exec_create_files(ast) == FUNCT_ERROR)
		return (FUNCT_ERROR);
	if (ast->left->type == PIPE)
	{
		if (exec_pipe_sequence(ast->left, data) == FUNCT_ERROR)
			return (FUNCT_ERROR);
	}
	else
	{
		if (exec_command(ast->left, data) == FUNCT_ERROR)
			return (FUNCT_ERROR);
	}
	return (exec_command(ast->right, data));
}

int				exec_and_or(t_ast *ast, t_vshdata *data)
{
	int		bg;

	bg = 0;
	if (ast->type != AND_IF && ast->type != OR_IF)
	{
		if (data->jobs->active_job != NULL)
			jobs_launch_job(data->jobs->active_job); // @rob, why is this one necessary?
		data->jobs->active_job = NULL;
		return (exec_pipe_sequence(ast, data));
	}
	if (data->exec_flags & EXEC_BG)
		bg = 1;
	data->exec_flags &= ~EXEC_BG;
	if (exec_and_or(ast->left, data) == FUNCT_ERROR)
		return (FUNCT_ERROR);
	if (ast->type == AND_IF && g_state->exit_code != EXIT_SUCCESS)
		return (FUNCT_ERROR);
	else if (ast->type == OR_IF && g_state->exit_code == EXIT_SUCCESS)
		return (FUNCT_FAILURE);
	if (bg == 1)
		data->exec_flags |= EXEC_BG;
	if (exec_and_or(ast->right, data) == FUNCT_ERROR)
		return (FUNCT_ERROR);
	return (FUNCT_SUCCESS);
}

int				exec_list(t_ast *ast, t_vshdata *data)
{
	if (ast->type != BG && ast->type != SEMICOL)
		return (exec_and_or(ast, data));
	if (ast->type == BG)
		data->exec_flags |= EXEC_BG;
	if (exec_and_or(ast->left, data) == FUNCT_ERROR)
		return (FUNCT_ERROR);
	if (ast->right != NULL)
	{
		if (exec_list(ast->right, data) == FUNCT_ERROR)
			return (FUNCT_ERROR);
	}
	return (FUNCT_SUCCESS);
}

int				exec_complete_command(t_ast *ast, t_vshdata *data)
{
	data->exec_flags = 0;
	if (ast == NULL || exec_list(ast, data) == FUNCT_ERROR)
		return (FUNCT_ERROR);
	if (data->jobs->active_job != NULL)
		jobs_launch_job(data->jobs->active_job);
	data->jobs->active_job = NULL;
	return (FUNCT_SUCCESS);
}
