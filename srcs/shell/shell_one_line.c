/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   shell_one_line.c                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: mavan-he <mavan-he@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/09/15 21:53:03 by mavan-he       #+#    #+#                */
/*   Updated: 2019/09/16 16:38:10 by mavan-he      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"

void	shell_one_line(t_vshdata *data)
{
	t_tokenlst	*token_lst;
	t_ast		*ast;

	token_lst = NULL;
	ast = NULL;
	if (history_expansion(data) != FUNCT_SUCCESS ||
		lexer(&data->line->line, &token_lst) != FUNCT_SUCCESS ||
		alias_expansion(data, &token_lst, NULL) != FUNCT_SUCCESS ||
		token_lst->next->type == NEWLINE ||
		parser_start(&token_lst, &ast) != FUNCT_SUCCESS)
	{
		g_state->exit_code = EXIT_FAILURE;
		return ;
	}
	exec_complete_command(ast, data);
	return ;
}
