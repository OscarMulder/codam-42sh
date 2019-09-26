/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   exec_quote_remove.c                                :+:    :+:            */
/*                                                     +:+                    */
/*   By: mavan-he <mavan-he@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/07/13 11:20:18 by mavan-he       #+#    #+#                */
/*   Updated: 2019/09/26 11:42:54 by tde-jong      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"

void		exec_quote_remove(t_ast *node)
{
	char	*str;
	bool	is_heredoc;

	if (node->left != NULL)
		exec_quote_remove(node->left);
	if (node->right != NULL)
		exec_quote_remove(node->right);
	if ((node->type == WORD || node->type == ASSIGN)
	&& (node->flags & T_FLAG_HEREDOC_NOEXP) == false)
	{
		is_heredoc = false;
		if (node->flags & T_FLAG_ISHEREDOC)
			is_heredoc = true;
		str = node->value;
		if (node->type == ASSIGN)
			str = ft_strchr(str, '=');
		tools_remove_quotes_etc(str, is_heredoc);
	}
}
