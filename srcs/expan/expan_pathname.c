/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   expan_pathname.c                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: mavan-he <mavan-he@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/10/18 12:06:36 by mavan-he       #+#    #+#                */
/*   Updated: 2019/10/18 16:57:18 by mavan-he      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"

static void	copy_first_expan_to_ast(t_ast *ast, t_ast **expan)
{
	t_ast *to_free;

	ast->flags = 0;
	ft_strdel(&ast->value);
	ast->value = (*expan)->value;
	ast->left = (*expan)->left;
	to_free = *expan;
	*expan = (*expan)->left;
	free(to_free);
}

static void	insert_expan_into_ast(t_ast **ast, t_ast *expan)
{
	t_ast *tmp;
	
	tmp = (*ast)->left;
	if (expan == NULL)
	{
		(*ast) = (*ast)->left;
		return ;
	}
	copy_first_expan_to_ast(*ast, &expan);
	if (expan == NULL)
	{
		(*ast)->left = tmp;
		*ast = tmp;
		return ;
	}
	while (expan->left != NULL)
		expan = expan->left;
	expan->left = tmp;
	*ast = tmp;
}

int		expan_pathname(t_ast *ast)
{
	t_ast	*expan;

	while (ast != NULL)
	{
		expan = NULL;
		if (glob_expand_word(&expan, ast->value) == FUNCT_ERROR)
			return (FUNCT_ERROR);
		insert_expan_into_ast(&ast, expan);
	}
	return (FUNCT_SUCCESS);
}
