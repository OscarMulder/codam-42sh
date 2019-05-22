/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parser_utils.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: mavan-he <mavan-he@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/05/21 19:54:55 by mavan-he       #+#    #+#                */
/*   Updated: 2019/05/21 21:01:24 by mavan-he      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"

t_ast			*new_ast_node(t_tokenlst *token)
{
	t_ast *node;

	node = (t_ast*)ft_memalloc(sizeof(t_ast));
	node->type = token->type;
	node->value = token->value;
	node->child = NULL;
	node->sibling = NULL;
	return (node);
}

int		add_sibling(t_tokenlst **token_lst, t_ast **ast,
		int (*parse_priority_x)(t_tokenlst **, t_ast **))
{
	t_ast *sibling;

	sibling = NULL;
	if (parse_priority_x(token_lst, &sibling) != FUNCT_SUCCESS)
		return (FUNCT_FAILURE);	
	(*ast)->sibling = sibling;
	return (FUNCT_SUCCESS);

}

int		add_astnode(t_tokenlst **token_lst, t_ast **ast)
{
	t_ast *new_node;

	new_node = new_ast_node(*token_lst);
	new_node->child = *ast;
	new_node->sibling = NULL;
	*ast = new_node;
	*token_lst = (*token_lst)->next;
	return (FUNCT_SUCCESS);
}
