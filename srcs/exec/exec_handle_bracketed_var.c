/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   exec_handle_bracketed_var.c                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: jbrinksm <jbrinksm@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/07/14 01:02:46 by jbrinksm       #+#    #+#                */
/*   Updated: 2019/07/14 01:18:18 by jbrinksm      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"

/*
**	Right now this function doesn't deal with brackets properly
**	because it does not care if it find a closing bracket.
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

	/* If there is a closing bracket we will skip it when
	copying the right side of the parameter var we just inserted */
	if (item->value[start + len] == '}')
		ft_strcat(new, &item->value[start + len + 1]);
	else
		ft_strcat(new, &item->value[start + len]);

	ft_strdel(&item->value);
	item->value = new;
	ft_printf("RESULT bracketed: %s\n", new);
	return (FUNCT_SUCCESS);
}

int			exec_handle_bracketed_var(t_ast *item, int *i, t_envlst *envlst)
{
	int		i_offset;
	char	*str;
	char	*identifier;
	char	*val;

	i_offset = *i;
	str = item->value;
	/* skip '$' and '{' */
	*i += 2;

	while (tools_isidentifierchar(str[*i]) == true)
		(*i)++;
	identifier = ft_strndup(&str[i_offset + 2], *i - (i_offset + 2));
	if (identifier == NULL)
		return (FUNCT_FAILURE);
	val = env_getvalue(identifier, envlst);
	ft_strdel(&identifier);
	if (repl_bracketed_var(item, val, i_offset, *i - i_offset) != FUNCT_SUCCESS)
		return (FUNCT_FAILURE);

	/* offsets parent 'i' with length if val, so it doesn't check
	it for '$' later */
	if (val != NULL)
		i_offset += ft_strlen(val);
	*i = i_offset; //is this correct?
	return (FUNCT_SUCCESS);
}
