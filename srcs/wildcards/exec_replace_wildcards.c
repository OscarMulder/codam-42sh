/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   exec_replace_wildcards.c                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: jbrinksm <jbrinksm@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/07/07 11:09:03 by jbrinksm       #+#    #+#                */
/*   Updated: 2019/07/07 18:13:30 by jbrinksm      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"

/*
**	The new list items should have a new type, since we don't want these
**	to be stripped of quotes once again after the wildcard expansion.
*/

static void	exec_match_wildcards(t_ast *item, int index)
{
	char	*left;
	char	*right;
}

static void	exec_scan_wildcards(t_ast *item)
{
	char	*value;
	char	quote;
	int		i;

	if (item == NULL || item->type != WORD)
		return ;
	i = 0;
	quote = '\0';
	while (item->value[i] != '\0')
	{
		tools_update_quote_status(item->value, i, &quote);
		if (item->value[i] == '*' && quote == '\0' &&
			tools_is_char_escaped(item->value, i))
		{
			exec_match_wildcards(item, i);
			i = 0;
			continue;
		}
		i++;
	}
}

/*
**	ASSIGN's are not yet supported.
*/

void	exec_handle_wildcards(t_ast *complete_command)
{
	t_ast	*probe;

	if (complete_command == NULL)
		return ;
	probe = complete_command;
	if (complete_command->type != WORD)
		return ;
	while (probe != NULL)
	{
		if (probe->type == WORD)
			exec_scan_wildcards(probe);
		#ifdef DEBUG
		else
		{
			ft_printf("warning: exec replace_wildcards found a %s\n",
			parser_return_token_str(probe->type));
		}
		#endif
		probe = probe->child;
	}
}