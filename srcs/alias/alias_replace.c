/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   alias_replace.c                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: mavan-he <mavan-he@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/07/26 20:29:50 by mavan-he       #+#    #+#                */
/*   Updated: 2019/07/26 23:02:19 by mavan-he      ########   odam.nl         */
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

static bool	is_cmd_seperator(t_tokens type)
{
	return (type == PIPE || type == AND_IF || type == OR_IF || type == BG ||
		type == SEMICOL || type == NEWLINE);
}

int			alias_replace(t_vshdata *vshdata, t_tokenlst *probe, char *alias)
{
	char		*new_line;
	char		*alias_equal;
	t_tokenlst	*new_tokenlst;
	t_tokenlst	*new_probe;
	int			status; // WHY
	
	status = 1;
	alias_equal = ft_strchr(alias, '=');
	new_line = ft_strdup(alias_equal + 1);
	new_tokenlst = NULL;
	if (new_line == NULL || shell_quote_checker(&new_line, &status)
		== FUNCT_ERROR || lexer(&new_line, &new_tokenlst) != FUNCT_SUCCESS ||
		shell_dless_input(new_tokenlst) != FUNCT_SUCCESS)
		return (FUNCT_ERROR);
	if (ft_strnequ(new_tokenlst->next->value, alias, alias_equal - alias)
		== true && new_tokenlst->next->value[alias_equal - alias] == '\0')
	{
		new_probe = new_tokenlst;
		while (new_probe->next->type != END &&
			is_cmd_seperator(new_probe->next->type) == false)
			new_probe = new_probe->next;
		if (alias_expansion(vshdata, &new_probe) == FUNCT_ERROR) 
			return (FUNCT_ERROR);
	}
	else if (alias_expansion(vshdata, &new_tokenlst) == FUNCT_ERROR)
		return (FUNCT_ERROR);
	alias_set(probe, new_tokenlst);
	return (FUNCT_SUCCESS);
}
