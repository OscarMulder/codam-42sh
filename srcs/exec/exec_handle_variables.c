/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   exec_handle_variables.c                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: jbrinksm <jbrinksm@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/07/07 20:54:47 by jbrinksm       #+#    #+#                */
/*   Updated: 2019/07/14 00:25:01 by jbrinksm      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"

static int	repl_regular_var(t_ast *item, char *val, int start, int len)
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
	ft_strcat(new, &item->value[start + len]);
	ft_strdel(&item->value);
	item->value = new;
	return (FUNCT_SUCCESS);
}

static int	handle_regular_var(t_ast *item, int *i, t_envlst *envlst)
{
	int		i_offset;
	char	*str;
	char	*identifier;
	char	*val;

	i_offset = *i;
	str = item->value;
	(*i)++;
	while (tools_isidentifierchar(str[*i]) == true)
		(*i)++;
	identifier = ft_strndup(&str[i_offset + 1], *i - (i_offset + 1));
	if (identifier == NULL)
		return (FUNCT_FAILURE);
	val = env_getvalue(identifier, envlst);
	if (repl_regular_var(item, val, i_offset, *i - i_offset) != FUNCT_SUCCESS)
		return (FUNCT_FAILURE);
	if (val != NULL)
		i_offset += ft_strlen(val) - 1;
	*i = i_offset; //is this correct?
	return (FUNCT_SUCCESS);
}

static int	scan_var(t_ast *item, t_envlst *envlst)
{
	char	quote;
	int		i;
	int		ret;

	if (item == NULL || item->type != WORD)
		return (FUNCT_FAILURE);
	i = 0;
	quote = '\0';
	while (item->value[i] != '\0')
	{
		tools_update_quote_status(item->value, i, &quote);
		if (item->value[i] == '$' && quote != '\'' &&
			tools_is_char_escaped(item->value, i) == false)
		{
			if (item->value[i + 1] != '{')
				ret = handle_regular_var(item, &i, envlst);
			// else
			// 	ret = handle_bracketed_var(item, &i, envlst);
			if (ret != FUNCT_SUCCESS)
				return (FUNCT_FAILURE);
		}
		i++;
	}
	return (FUNCT_SUCCESS);
}

/*
**	ASSIGN's are not yet supported.
*/

int		exec_handle_variables(t_ast *complete_command, t_envlst *envlst)
{
	t_ast	*probe;

	if (complete_command == NULL)
		return (FUNCT_FAILURE);
	probe = complete_command;
	if (complete_command->type != WORD)
		return (FUNCT_FAILURE);
	while (probe != NULL)
	{
		if (probe->type == WORD)
		{
			if (scan_var(probe, envlst) != FUNCT_SUCCESS)
				return (FUNCT_FAILURE);
		}
		#ifdef DEBUG
		else
		{
			ft_printf("warning: exec_replace_variables found a %s\n",
			parser_return_token_str(probe->type));
		}
		#endif
		probe = probe->child;
	}
	return (FUNCT_SUCCESS);
}