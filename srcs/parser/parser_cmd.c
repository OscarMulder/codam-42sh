/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parser_cmd.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: mavan-he <mavan-he@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/05/25 19:13:12 by mavan-he       #+#    #+#                */
/*   Updated: 2019/05/25 19:19:37 by mavan-he      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"

bool	add_redir_node(t_tokenlst **token_lst, t_ast **ast)
{
	t_ast *redir;

	redir = NULL;
	if (TK_TYPE == IO_NUMBER && add_astnode(token_lst, ast) == false)
		return (false);
	if (is_redirect_tk(TK_TYPE) == false ||
		add_astnode(token_lst, ast) == false)
		return (false);
	if ((TK_TYPE != WORD && TK_TYPE != ASSIGN) ||
		add_astnode(token_lst, &redir) == false)
		return (false);
	if ((*ast)->child == NULL)
		(*ast)->child = redir;
	else
		(*ast)->child->child = redir;
	(*ast)->sibling = (*ast)->child;
	(*ast)->child = NULL;
	return (true);
}

bool	cmd_suffix(t_tokenlst **token_lst, t_ast **suffix, t_ast **prefix)
{
	t_ast *next_ast;

	next_ast = NULL;
	if (TK_TYPE == IO_NUMBER || is_redirect_tk(TK_TYPE) == true)
	{
		if (add_redir_node(token_lst, &next_ast) == false)
			return (false);
		if (*prefix == NULL)
			*prefix = next_ast;
		else
			(*prefix)->child = next_ast;
		if (cmd_suffix(token_lst, suffix, &next_ast) == false)
			return (false);
	}
	else if (TK_TYPE == WORD || TK_TYPE == ASSIGN)
	{
		if (add_astnode(token_lst, &next_ast) == false)
			return (false);
		if (*suffix == NULL)
			*suffix = next_ast;
		else
			(*suffix)->child = next_ast;
		if (cmd_suffix(token_lst, &next_ast, prefix) == false)
			return (false);
	}
	return (true);
}

bool	cmd_prefix(t_tokenlst **token_lst, t_ast **ast, t_ast **prefix)
{
	t_ast *next_prefix;

	next_prefix = NULL;
	if (TK_TYPE == ASSIGN || TK_TYPE == IO_NUMBER ||
		is_redirect_tk(TK_TYPE) == true)
	{
		if (TK_TYPE == ASSIGN && add_astnode(token_lst, ast) == false)
			return (false);
		else if (add_redir_node(token_lst, ast) == false)
			return (false);
		*prefix = *ast;
		if (cmd_prefix(token_lst, &next_prefix, prefix) == false)
			return (false);
		(*ast)->child = next_prefix;
	}
	if (TK_TYPE != WORD && *ast == NULL)
		return (false);
	return (true);
}

bool	cmd_word(t_tokenlst **token_lst, t_ast **ast)
{
	if (TK_TYPE == WORD)
	{
		if (add_astnode(token_lst, ast) == false)
			return (false);
		return (true);
	}
	else if (*ast != NULL)
		return (true);
	else
		return (false);
}

bool	cmd(t_tokenlst **token_lst, t_ast **ast)
{
	t_ast	*prefix;
	t_ast	*suffix;

	prefix = NULL;
	suffix = NULL;
	if (TK_TYPE == WORD || TK_TYPE == ASSIGN || TK_TYPE == IO_NUMBER ||
		is_redirect_tk(TK_TYPE) == true)
	{
		if (cmd_prefix(token_lst, ast, &prefix) == false)
			return (false);
		if (cmd_word(token_lst, ast) == false)
			return (false);
		if (cmd_suffix(token_lst, &suffix, &prefix) == false)
			return (false);
		if ((*ast)->child == NULL)
			(*ast)->child = prefix;
		(*ast)->sibling = suffix;
		return (true);
	}
	else
		return (false);
}
