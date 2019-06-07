/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   builtin_assign.c                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: jbrinksm <jbrinksm@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/06/05 09:09:49 by jbrinksm       #+#    #+#                */
/*   Updated: 2019/06/05 09:30:53 by jbrinksm      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"

/*
**	NOT SURE IF CORRECT ASSUMPTIONS AS OF HOW IT IS SUPPOSED TO WORK:
**	Changes the envlst contents based on arg.
**	If a new lst item has to be made, the variable will be defaulted
**	to ENV_LOCAL. If the variable already is ENV_EXTERN it's value
**	will be changed and it will remain ENV_EXTERN.
*/

void	builtin_assign(char *arg, t_envlst *envlst, int *exit_code)
{
	t_envlst	*probe;
	int			varlen;
	char		*var;

	probe = envlst;
	*exit_code = EXIT_FAILURE;
	if (probe == NULL || arg == NULL)
		return ;
	var = ft_strdup(arg);
	if (var == NULL)
		return ;
	*exit_code = EXIT_SUCCESS;
	varlen = ft_strclen(arg, '=');
	while (probe->next != NULL)
	{
		if (ft_strncmp(arg, probe->var, varlen + 1) == 0)
		{
			probe->var = var;
			return ;
		}
		probe = probe->next;
	}
	probe->next = env_lstnew(var, ENV_LOCAL);
	ft_strdel(&var);
}
