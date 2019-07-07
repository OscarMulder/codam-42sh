/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   exec_replace_variables.c                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: jbrinksm <jbrinksm@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/07/07 20:54:47 by jbrinksm       #+#    #+#                */
/*   Updated: 2019/07/07 21:22:23 by jbrinksm      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"

static void	exec_match_variables(t_ast *item, int index, t_list **matches)
{
	if (*matches == NULL)
		*matches = ft_lstnew(item->value, ft_strlen(item->value) + 1);
	else
	{

	}
}

static void	exec_scan_variables(t_ast *item)
{
	char	*value;
	char	quote;
	int		i;
	t_list	*matches;

	if (item == NULL || item->type != WORD)
		return ;
	i = 0;
	quote = '\0';
	matches = NULL;
	while (item->value[i] != '\0')
	{
		tools_update_quote_status(item->value, i, &quote);
		if (item->value[i] == '$' && quote == '\0' &&
			tools_is_char_escaped(item->value, i))
		{
			exec_match_wildcards(item, i, &matches);
			i = 0;
			continue;
		}
		i++;
	}
}

/*
**	ASSIGN's are not yet supported.
*/

void	exec_handle_variables(t_ast *complete_command)
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
			exec_scan_variables(probe);
		#ifdef DEBUG
		else
		{
			ft_printf("warning: exec_replace_variables found a %s\n",
			parser_return_token_str(probe->type));
		}
		#endif
		probe = probe->child;
	}
}