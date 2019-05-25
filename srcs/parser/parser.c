/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parser.c                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: jbrinksm <jbrinksm@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/05/19 19:58:40 by jbrinksm       #+#    #+#                */
/*   Updated: 2019/05/25 19:19:06 by mavan-he      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"

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
		if ((*token_lst)->flags & T_MALLOC_ERROR)
			ft_putstr("vsh: parser: malloc error\n");
		else
			ft_printf("vsh: syntax error near unexpected token `%s'\n",
			(*token_lst)->value); // display correct token value
	}
	else
		print_tree(ast);
	return (0);
}
