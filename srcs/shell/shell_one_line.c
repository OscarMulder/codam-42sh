/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   shell_one_line.c                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: mavan-he <mavan-he@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/09/15 21:53:03 by mavan-he       #+#    #+#                */
/*   Updated: 2019/09/18 15:34:13 by mavan-he      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"

/*
** This will be replaced by a cmd line splitter
** Currently we only handle one line and stop at the first newline
*/

int		shell_one_line(t_vshdata *data)
{
	t_tokenlst	*token_lst;
	t_ast		*ast;

	token_lst = NULL;
	ast = NULL;
	if (history_expansion(data) != FUNCT_SUCCESS ||
		lexer(&data->line->line, &token_lst) != FUNCT_SUCCESS ||
		alias_expansion(data, &token_lst, NULL) != FUNCT_SUCCESS ||
		token_lst->next->type == NEWLINE ||
		parser_start(&token_lst, &ast) != FUNCT_SUCCESS ||
		exec_complete_command(ast, data) == FUNCT_ERROR)
	{
		shell_clear_input_data(&data->line->line, &ast, &token_lst);
		g_state->exit_code = EXIT_FAILURE;
		return (FUNCT_ERROR);
	}
	shell_clear_input_data(&data->line->line, &ast, &token_lst);
	return (FUNCT_SUCCESS);
}
