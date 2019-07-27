/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   alias_replace.c                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: mavan-he <mavan-he@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/07/26 20:29:50 by mavan-he       #+#    #+#                */
/*   Updated: 2019/07/27 17:29:08 by mavan-he      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"

static void	alias_set(t_tokenlst *probe, t_tokenlst *new_tokenlst)
{
	t_tokenlst *start_end;

	start_end = new_tokenlst;
	new_tokenlst = new_tokenlst->next;
	ft_memdel((void**)&start_end);
	start_end = new_tokenlst;
	while (start_end->next->type != END)
		start_end = start_end->next;
	ft_memdel((void**)&start_end->next);
	start_end->next = probe->next->next;
	ft_memdel((void**)&probe->next);
	probe->next = new_tokenlst;
}

char		**alias_add_expanded(char **expanded_aliases, char *alias, char *alias_equal)
{
	char		**new_expanded_aliases;
	char		*alias_key;
	int			i;
	int			new_i;

	i = 0;
	if (expanded_aliases == NULL)
		new_expanded_aliases = (char **)ft_memalloc(sizeof(char *) * 2);
	else
	{
		while (expanded_aliases[i] != NULL)
			i++;
		new_expanded_aliases = (char **)ft_memalloc(sizeof(char *) * (i + 2));
	}
	alias_key = ft_strndup(alias, alias_equal - alias);
	if (new_expanded_aliases == NULL || alias_key == NULL)
		return (NULL);
	new_i = 0;
	while (new_i < i)
	{
		new_expanded_aliases[new_i] = ft_strdup(expanded_aliases[new_i]);
		new_i++;
		// malloc check
	}
	new_expanded_aliases[new_i] = alias_key;
	return (new_expanded_aliases);
}

int			alias_replace(t_vshdata *vshdata, t_tokenlst *probe, char *alias, char **expanded_aliases)
{
	char		*new_line;
	char		*alias_equal;
	char		**new_expanded_aliases;
	t_tokenlst	*new_tokenlst;
	int			status; // WHY
	
	status = 1;
	alias_equal = ft_strchr(alias, '=');
	new_line = ft_strdup(alias_equal + 1);
	new_tokenlst = NULL;
	if (new_line == NULL || shell_quote_checker(&new_line, &status)
		== FUNCT_ERROR || lexer(&new_line, &new_tokenlst) != FUNCT_SUCCESS ||
		shell_dless_input(new_tokenlst) != FUNCT_SUCCESS)
		return (FUNCT_ERROR);
	new_expanded_aliases = alias_add_expanded(expanded_aliases, alias, alias_equal);
	if (new_expanded_aliases == NULL)
		return (FUNCT_ERROR);
	if (alias_expansion(vshdata, &new_tokenlst, new_expanded_aliases) == FUNCT_ERROR)
		return (FUNCT_ERROR);
	alias_set(probe, new_tokenlst);
	return (FUNCT_SUCCESS);
}
