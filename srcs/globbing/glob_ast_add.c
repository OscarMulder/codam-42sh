/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   glob_ast_add.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: mavan-he <mavan-he@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/10/18 14:38:28 by mavan-he       #+#    #+#                */
/*   Updated: 2019/10/18 18:11:59 by mavan-he      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"

static t_ast	*ast_new_node(char *value, t_tokens type, char flags)
{
	t_ast *node;

	node = (t_ast*)ft_memalloc(sizeof(t_ast));
	if (node == NULL)
		return (NULL);
	node->type = type;
	node->flags = flags;
	if (value != NULL)
	{
		node->value = ft_strdup(value);
		if (node->value == NULL)
			return (NULL);
	}
	else
		node->value = NULL;
	return (node);
}

int				glob_ast_add_left(t_ast **ast, char *value,
				t_tokens type, char flags)
{
	t_ast	*new_node;
	t_ast	*probe;

	new_node = ast_new_node(value, type, flags);
	if (new_node == NULL)
		return (FUNCT_ERROR);
	if (*ast == NULL)
		*ast = new_node;
	else
	{
		probe = *ast;
		while (probe->left != NULL)
			probe = probe->left;
		probe->left = new_node;
	}
	return (FUNCT_SUCCESS);
}
