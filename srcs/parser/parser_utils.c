/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parser_utils.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: mavan-he <mavan-he@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/05/21 19:54:55 by mavan-he       #+#    #+#                */
/*   Updated: 2019/05/26 12:14:51 by mavan-he      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"

t_ast	*new_ast_node(t_tokenlst *token)
{
	t_ast *node;

	node = (t_ast*)ft_memalloc(sizeof(t_ast));
	if (node == NULL)
		return (NULL);
	node->type = token->type;
	node->flags = token->flags;
	node->value = token->value;
	node->child = NULL;
	node->sibling = NULL;
	return (node);
}

bool	add_sibling(t_tokenlst **token_lst, t_ast **ast,
		bool (*parse_priority_x)(t_tokenlst **, t_ast **))
{
	t_ast *sibling;

	sibling = NULL;
	if (parse_priority_x(token_lst, &sibling) != true)
		return (false);
	(*ast)->sibling = sibling;
	return (true);
}

bool	add_astnode(t_tokenlst **token_lst, t_ast **ast)
{
	t_ast *new_node;

	new_node = new_ast_node(*token_lst);
	if (new_node == NULL)
	{
		(*token_lst)->flags |= T_MALLOC_ERROR;
		return (false);
	}
	new_node->child = *ast;
	new_node->sibling = NULL;
	*ast = new_node;
	*token_lst = (*token_lst)->next;
	return (true);
}

bool	is_redirect_tk(t_tokens type)
{
	return (type == SLESS || type == SGREAT || type == DLESS ||
			type == DGREAT || type == LESSAND || type == GREATAND);
}
