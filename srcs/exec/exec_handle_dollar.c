/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   exec_handle_dollar.c                               :+:    :+:            */
/*                                                     +:+                    */
/*   By: jbrinksm <jbrinksm@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/07/14 01:05:00 by jbrinksm       #+#    #+#                */
/*   Updated: 2019/07/31 15:45:53 by mavan-he      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"

/*
**	Goal of the function is to take the expansion sequence and
**	replace it with the value of the parameter.
*/

static int	repl_regular_var(char **value, char *val, int start, int len)
{
	char	*new;
	int		val_len;

	if (val != NULL)
		val_len = ft_strlen(val);
	else
		val_len = 0;
	new = ft_strnew(ft_strlen(*value) - len + val_len);
	if (new == NULL)
		return (FUNCT_ERROR);
	ft_strncpy(new, *value, start);
	if (val != NULL)
		ft_strcat(new, val);
	ft_strcat(new, &(*value)[start + len]);
	ft_strdel(value);
	*value = new;
	return (FUNCT_SUCCESS);
}

/*
**	Goal of this function is to extract the identifier
**	and get its value (or nothing if it doesn't exist)
*/

int			exec_handle_dollar(char **value, int *i, t_envlst *envlst)
{
	int		i_offset;
	char	*identifier;
	char	*replace_str;

	if ((*value)[*i + 1] == '{')
		return (exec_handle_bracketed_var(value, i, envlst));
	i_offset = *i;
	(*i)++;
	while (tools_isidentifierchar((*value)[*i]) == true)
		(*i)++;
	if (*i == i_offset + 1)
		return (FUNCT_FAILURE);
	identifier = ft_strndup(&(*value)[i_offset + 1], *i - (i_offset + 1));
	if (identifier == NULL)
		return (FUNCT_ERROR);
	replace_str = env_getvalue(identifier, envlst);
	ft_strdel(&identifier);
	if (repl_regular_var(value, replace_str, i_offset, *i - i_offset) == FUNCT_ERROR)
		return (FUNCT_ERROR);
	if (replace_str != NULL)
		i_offset += ft_strlen(replace_str);
	*i = i_offset;
	return (FUNCT_SUCCESS);
}
