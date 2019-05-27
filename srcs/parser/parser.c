/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parser.c                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: jbrinksm <jbrinksm@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/05/19 19:58:40 by jbrinksm       #+#    #+#                */
/*   Updated: 2019/05/27 15:20:10 by omulder       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"

static bool	pipe_sequence(t_tokenlst **token_lst, t_ast **ast)
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

static bool	and_or(t_tokenlst **token_lst, t_ast **ast)
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

static bool	list(t_tokenlst **token_lst, t_ast **ast)
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

static bool	complete_cmd(t_tokenlst **token_lst, t_ast **ast)
{
	if (list(token_lst, ast) == true && TK_TYPE == END)
		return (true);
	return (false);
}

int		parser(t_tokenlst **token_lst, t_ast **ast)
{
	t_tokenlst	*tmp;

	tmp = (*token_lst)->next;
	if (complete_cmd(&tmp, ast) != true)
	{
		if ((tmp)->flags & T_MALLOC_ERROR)
			ft_putstr("vsh: parser: malloc error\n");
		else
			ft_printf("vsh: syntax error near unexpected token `%s'\n",
			return_token_str((tmp)->type));
		tokenlstdel(token_lst);
		astdel(ast);
		return (FUNCT_FAILURE);
	}
	tokenlstdel(token_lst);
	return (FUNCT_SUCCESS);
}
