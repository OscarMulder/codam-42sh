/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   exec_handle_variables.c                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: jbrinksm <jbrinksm@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/07/07 20:54:47 by jbrinksm       #+#    #+#                */
/*   Updated: 2019/07/14 01:19:50 by jbrinksm      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"

/*
**	Finds unquoted and unescaped '$', then checks if
**	it is a bracketed parameter/variable or not,
**	and then executes the proper functions on it.
**
**	Notice the '&i' gets sent with these functions, because
**	when it is done, it will point to the characted after
**	the var with which the $PARAM was just replaced. (to avoid
**	faulty param replacement).
*/

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
			if (item->value[i + 1] == '{')
				ret = exec_handle_bracketed_var(item, &i, envlst);
			else
			{
				// ADD BRACKET FORCE CLOSER HERE (think dless_input)
				ret = exec_handle_regular_var(item, &i, envlst);
			}
			if (ret != FUNCT_SUCCESS)
				return (FUNCT_FAILURE);
			continue ;
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
			ft_printf("warning: exec_handle_variables found a %s\n",
			parser_return_token_str(probe->type));
		}
		#endif
		probe = probe->child;
	}
	return (FUNCT_SUCCESS);
}
