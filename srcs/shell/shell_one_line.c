/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   shell_one_line.c                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: mavan-he <mavan-he@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/09/15 21:53:03 by mavan-he       #+#    #+#                */
/*   Updated: 2019/09/15 22:32:19 by mavan-he      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"

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
		parser_start(&token_lst, &ast) != FUNCT_SUCCESS)
		return (FUNCT_ERROR);
	exec_complete_command(ast, data);
	return (FUNCT_SUCCESS);
}
