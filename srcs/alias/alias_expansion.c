/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   alias_expansion.c                                  :+:    :+:            */
/*                                                     +:+                    */
/*   By: mavan-he <mavan-he@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/07/25 17:24:39 by mavan-he       #+#    #+#                */
/*   Updated: 2019/07/27 17:28:53 by mavan-he      ########   odam.nl         */
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

static bool	alias_space_check(char *alias)
{
	char c;

	c = alias[ft_strlen(alias) - 1];
	return (c == '\t' || c == ' ');
}

static char	*alias_find_value(char *key, t_aliaslst *aliaslst, char **expanded_aliases)
{
	int		key_len;
	int		i;

	if (expanded_aliases != NULL)
	{
		i = 0;
		while (expanded_aliases[i] != NULL)
		{
			if (ft_strequ(expanded_aliases[i], key) == true)
				return (NULL);
			i++;
		}
	}
	key_len = ft_strlen(key);
	while (aliaslst != NULL)
	{
		if (ft_strnequ(key, aliaslst->var, key_len) == true &&
			(aliaslst->var[key_len] == '='))
			return (aliaslst->var);
		aliaslst = aliaslst->next;
	}
	return (NULL);
}

int			alias_expansion_checker(t_vshdata *vshdata, t_tokenlst *probe,
			char **expanded_aliases)
{
	t_tokenlst	*new_probe;
	char		*alias;

	alias = alias_find_value(probe->next->value, vshdata->aliaslst, expanded_aliases);
	if (alias != NULL)
	{
		if (alias_space_check(alias) == true)
		{
			ft_printf("Replace %s in has space\n", alias); // testing
			if (alias_expansion(vshdata, &probe->next, expanded_aliases) == FUNCT_ERROR)
				return (FUNCT_ERROR);
		}
		else
		{
			new_probe = probe;
			while (new_probe->next->type != END &&
				is_cmd_seperator(new_probe->next->type) == false)
				new_probe = new_probe->next;
			ft_printf("Will try to replace probe after %s\n", new_probe->value); // testing
			if (alias_expansion(vshdata, &new_probe, expanded_aliases) == FUNCT_ERROR)
				return (FUNCT_ERROR);
		}
		alias_replace(vshdata, probe, alias, expanded_aliases);
		return (FUNCT_SUCCESS);
	}
	else
		return (FUNCT_FAILURE);
}

int			alias_expansion(t_vshdata *vshdata, t_tokenlst **tokenlst,
			char **expanded_aliases)
{
	t_tokenlst	*probe;
	int			ret;
	
	probe = *tokenlst;
	while (probe->type != END && probe->next->type != END)
	{
		if (probe->next->type == WORD &&
			(probe->flags & T_FLAG_HASSPECIAL) == false)
		{
			ret = alias_expansion_checker(vshdata, probe, expanded_aliases);
			if (ret == FUNCT_ERROR)
				return (FUNCT_ERROR); // del tokenlst
			if (ret == FUNCT_SUCCESS)
				return (FUNCT_SUCCESS);
		}
		while (probe->next->type != END &&
			is_cmd_seperator(probe->next->type) == false)
			probe = probe->next;
		probe = probe->next;
	}
	return (FUNCT_SUCCESS);
}
