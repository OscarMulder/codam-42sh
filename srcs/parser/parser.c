/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parser.c                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: jbrinksm <jbrinksm@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/05/19 19:58:40 by jbrinksm       #+#    #+#                */
/*   Updated: 2019/05/22 12:05:01 by mavan-he      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"

int		cmd_word(t_tokenlst **token_lst, t_ast **ast)
{
	if ((*token_lst)->type == WORD ||
		(*token_lst)->type == ASSIGN)
			add_astnode(token_lst, ast); 
	else
		return (FUNCT_FAILURE);
	return (FUNCT_SUCCESS);
}

int		list(t_tokenlst **token_lst, t_ast **ast)
{
	if (cmd_word(token_lst, ast) == FUNCT_SUCCESS)
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
		ft_putstr("\n\nJammer dit \n");
	else
		ft_putstr("\n\nNice ? \n");
	print_tree(ast);
	return (0);
}
