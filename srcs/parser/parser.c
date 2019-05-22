/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parser.c                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: jbrinksm <jbrinksm@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/05/19 19:58:40 by jbrinksm       #+#    #+#                */
/*   Updated: 2019/05/22 18:31:23 by mavan-he      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"

int		cmd_word(t_tokenlst **token_lst, t_ast **ast)
{
	t_tokenlst *current;

	if ((*token_lst)->type == WORD || (*token_lst)->type == ASSIGN)
	{
		current = *token_lst;
		if ((*token_lst)->next->type == WORD || (*token_lst)->next->type == ASSIGN)
		{
			*token_lst = (*token_lst)->next;
			cmd_word(token_lst, ast);
			add_astnode(&current, ast);
		}
		else
			add_astnode(token_lst, ast);

	}
	else
		return (FUNCT_FAILURE);
	return (FUNCT_SUCCESS);
}

int		pipe_sequence(t_tokenlst **token_lst, t_ast **ast)
{
	if (cmd_word(token_lst, ast) == FUNCT_SUCCESS)
	{
		while ((*token_lst)->type == PIPE)
		{
			add_astnode(token_lst, ast);
			if (add_sibling(token_lst, ast, &cmd_word) != FUNCT_SUCCESS)
				return (FUNCT_FAILURE);
		}
		return (FUNCT_SUCCESS);
	}
	return (FUNCT_FAILURE);
}

int		and_or(t_tokenlst **token_lst, t_ast **ast)
{
	if (pipe_sequence(token_lst, ast) == FUNCT_SUCCESS)
	{
		while ((*token_lst)->type == AND_IF ||
			(*token_lst)->type == OR_IF)
		{
			add_astnode(token_lst, ast);
			if (add_sibling(token_lst, ast, &pipe_sequence) != FUNCT_SUCCESS)
				return (FUNCT_FAILURE);
		}
		return (FUNCT_SUCCESS);
	}
	return (FUNCT_FAILURE);
}

int		list(t_tokenlst **token_lst, t_ast **ast)
{
	if (and_or(token_lst, ast) == FUNCT_SUCCESS)
	{
		if ((*token_lst)->type == SEMICOL ||
			(*token_lst)->type == BG)
		{
			add_astnode(token_lst, ast);
			if ((*token_lst)->type != END)
			{
				if (add_sibling(token_lst, ast, &list) != FUNCT_SUCCESS)
					return (FUNCT_FAILURE);
			}
		}
		return (FUNCT_SUCCESS);
	}
	return (FUNCT_FAILURE);
}

int		complete_cmd(t_tokenlst **token_lst, t_ast **ast)
{
	if (list(token_lst, ast) == FUNCT_SUCCESS && (*token_lst)->type == END)
			return (FUNCT_SUCCESS);
	return (FUNCT_FAILURE);
}

int		parser(t_tokenlst **token_lst)
{
	t_ast *ast;

	*token_lst = (*token_lst)->next;
	ast = NULL;
	if (complete_cmd(token_lst, &ast) != FUNCT_SUCCESS)
	{
		ft_putstr("\n\nSomething went wrong near token value\n");
		ft_putendl((*token_lst)->value);
	}
	else
		print_tree(ast);
	return (0);
}
