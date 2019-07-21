/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   exec_handle_bracketed_var.c                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: jbrinksm <jbrinksm@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/07/14 01:02:46 by jbrinksm       #+#    #+#                */
/*   Updated: 2019/07/21 14:28:54 by jbrinksm      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"

/*
**	Goal of the function is to take the expansion sequence and
**	replace it with the value of the parameter.
*/

static int	repl_bracketed_var(t_ast *item, char *val, int start, int len)
{
	char	*new;
	int		val_len;

	if (val != NULL)
		val_len = ft_strlen(val);
	else
		val_len = 0;
	new = ft_strnew(ft_strlen(item->value) - len + val_len);
	if (new == NULL)
		return (FUNCT_FAILURE);
	ft_strncpy(new, item->value, start);
	if (val != NULL)
		ft_strcat(new, val);
	if (item->value[start + len] == '}')
		ft_strcat(new, &item->value[start + len + 1]);
	else
		ft_strcat(new, &item->value[start + len]);
	ft_strdel(&item->value);
	item->value = new;
	return (FUNCT_SUCCESS);
}

/*
**	Right now this function doesn't deal with brackets properly
**	because it does not care if it find a closing bracket or not.
**	It just stops when it finds a non-identifier char.
**
**	Goal of this function is to extract the identifier
**	and get its value (or nothing if it doesn't exist)
*/

int			exec_handle_bracketed_var(t_ast *item, int *i, t_envlst *envlst)
{
	int		i_offset;
	char	*str;
	char	*identifier;
	char	*val;

	if (item == NULL || item->value == NULL)
		return (FUNCT_FAILURE);
	i_offset = *i;
	str = item->value;
	*i += 2; // skip '$' and '{'
	while (tools_isidentifierchar(str[*i]) == true)
		(*i)++;
	identifier = ft_strndup(&str[i_offset + 2], *i - (i_offset + 2));
	if (identifier == NULL)
		return (FUNCT_FAILURE);
	val = env_getvalue(identifier, envlst);
	ft_strdel(&identifier);
	if (repl_bracketed_var(item, val, i_offset, *i - i_offset) != FUNCT_SUCCESS)
		return (FUNCT_FAILURE);
	if (val != NULL) 				// puts 'i' behind the var
		i_offset += ft_strlen(val);	// we just placed in the string
	*i = i_offset;
	return (FUNCT_SUCCESS);
}
