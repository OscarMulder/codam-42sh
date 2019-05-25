/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parser.c                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: jbrinksm <jbrinksm@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/05/19 19:58:40 by jbrinksm       #+#    #+#                */
/*   Updated: 2019/05/25 17:01:29 by mavan-he      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"

/* bool	add_redir_node(t_tokenlst **token_lst, t_ast **suffix, t_ast **prefix)
{
	t_ast *next_ast;

	next_ast = NULL;
	if (add_astnode(token_lst, &next_ast) == false)
		return (false);
	if (*prefix == NULL)
		*prefix = next_ast;
	else
		(*prefix)->child = next_ast;
	if (cmd_suffix(token_lst, suffix, &next_ast) == false)
		return (false);
} */

bool	cmd_suffix(t_tokenlst **token_lst, t_ast **suffix, t_ast **prefix)
{
	t_ast *next_ast;

	next_ast = NULL;
	if (TK_TYPE == IO_NUMBER || is_redirect_tk(TK_TYPE) == true)
	{
		if (add_astnode(token_lst, &next_ast) == false)
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
		if (add_astnode(token_lst, ast) == false)
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
		if (cmd_prefix(token_lst, ast, &prefix) == false) // flag != error
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

bool	pipe_sequence(t_tokenlst **token_lst, t_ast **ast)
{
	if (cmd(token_lst, ast) == true)
	{
		while (TK_TYPE == PIPE)
		{
			if (add_astnode(token_lst, ast) == false)
				return (false);
			if (add_sibling(token_lst, ast, &cmd) != true)
				return (false);
		}
		return (true);
	}
	return (false);
}

bool	and_or(t_tokenlst **token_lst, t_ast **ast)
{
	if (pipe_sequence(token_lst, ast) == true)
	{
		while (TK_TYPE == AND_IF ||
			TK_TYPE == OR_IF)
		{
			if (add_astnode(token_lst, ast) == false)
				return (false);
			if (add_sibling(token_lst, ast, &pipe_sequence) != true)
				return (false);
		}
		return (true);
	}
	return (false);
}

bool	list(t_tokenlst **token_lst, t_ast **ast)
{
	if (and_or(token_lst, ast) == true)
	{
		if (TK_TYPE == SEMICOL ||
			TK_TYPE == BG)
		{
			if (add_astnode(token_lst, ast) == false)
				return (false);
			if (TK_TYPE != END)
			{
				if (add_sibling(token_lst, ast, &list) == false)
					return (false);
			}
		}
		return (true);
	}
	return (false);
}

bool	complete_cmd(t_tokenlst **token_lst, t_ast **ast)
{
	if (list(token_lst, ast) == true && TK_TYPE == END)
			return (true);
	return (false);
}

int		parser(t_tokenlst **token_lst)
{
	t_ast *ast;

	*token_lst = (*token_lst)->next;
	ast = NULL;
	if (complete_cmd(token_lst, &ast) != true)
	{
		// if flag is error print malloc error
		ft_putstr("\n\nSomething went wrong near token value\n");
		ft_putendl((*token_lst)->value);
	}
	else
		print_tree(ast);
	return (0);
}
