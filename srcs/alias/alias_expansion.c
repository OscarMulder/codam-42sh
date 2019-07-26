/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   alias_expansion.c                                  :+:    :+:            */
/*                                                     +:+                    */
/*   By: mavan-he <mavan-he@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/07/25 17:24:39 by mavan-he       #+#    #+#                */
/*   Updated: 2019/07/26 17:12:00 by mavan-he      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"

// TO DO IN ALIAS:

// add escape sequence when value contains a single quote

// TO DO:

// Alias substitudes the first WORD in a simple command with it's value
// If the first WORD contains a special shell character it is skipped
// The first word of the expansion is checked again but only if it's not a duplicate
// if there is a space or tab at the end, the second WORD is checked as well
// reloading aliasses from shell start ?

// TO DO when replaced
// Call quote checker

static bool	is_cmd_seperator(t_tokens type)
{
	return (type == PIPE || type == AND_IF || type == OR_IF || type == BG ||
		type == SEMICOL || type == NEWLINE);
}

static char	*alias_find_value(char *key, t_aliaslst *aliaslst)
{
	int		key_len;

	key_len = ft_strlen(key);
	while (aliaslst != NULL)
	{
		if (ft_strnequ(key, aliaslst->var, key_len) == true &&
			(aliaslst->var[key_len] == '='))
			return (ft_strchr(aliaslst->var, '=') + 1);
		aliaslst = aliaslst->next;
	}
	return (NULL);
}

int			alias_expansion_checker(t_vshdata *vshdata, t_tokenlst *probe)
{
	// statusif alias exists
	// do all magic to get new tokenlst
	// call alias_expansion with current probe only if alias has space
	if (alias_find_value(probe->next->value, vshdata->aliaslst) != NULL)
	{
		ft_putstr(alias_find_value(probe->next->value, vshdata->aliaslst));
		return (FUNCT_SUCCESS);
	}
	return (FUNCT_FAILURE);
}

int			alias_expansion(t_vshdata *vshdata, t_tokenlst **tokenlst)
{
	t_tokenlst	*probe;
	
	probe = *tokenlst;
	while (probe->next->type != END)
	{
		if (probe->next->type == WORD &&
			(probe->flags & T_FLAG_HASSPECIAL) == false)
		{
			if (alias_expansion_checker(vshdata, probe) == FUNCT_ERROR)
				return (FUNCT_ERROR);
		}
		while (probe->next->type != END &&
			is_cmd_seperator(probe->next->type) == false)
			probe = probe->next;
		probe = probe->next;
	}
	return (FUNCT_SUCCESS);
}
