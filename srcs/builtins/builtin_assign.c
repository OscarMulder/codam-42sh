/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   builtin_assign.c                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: jbrinksm <jbrinksm@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/06/05 09:09:49 by jbrinksm       #+#    #+#                */
/*   Updated: 2019/06/07 17:53:46 by codam         ########   odam.nl         */
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

int		builtin_assign_addexist(t_envlst *envlst, char *arg, char *var)
{
	t_envlst	*probe;
	int			varlen;

	probe = envlst;
	varlen = ft_strclen(arg, '=');
	while (probe->next != NULL)
	{
		if (ft_strncmp(arg, probe->var, varlen) == 0 &&
		probe->var[varlen] == '=')
		{
			probe->var = var;
			return (FUNCT_SUCCESS);
		}
		probe = probe->next;
	}
	return (FUNCT_FAILURE);
}

int		builtin_assign_addnew(t_envlst *envlst, char *var)
{
	t_envlst	*newitem;

	newitem = env_lstnew(var, ENV_LOCAL);
	if (newitem == NULL)
		return (FUNCT_ERROR);
	env_lstaddback(&envlst, newitem);
	return (FUNCT_SUCCESS);
}

void	builtin_assign(char *arg, t_envlst *envlst, int *exit_code)
{
	char		*var;

	*exit_code = EXIT_FAILURE;
	if (envlst == NULL || arg == NULL)
		return ;
	var = ft_strdup(arg);
	if (var == NULL)
		return ;
	*exit_code = EXIT_SUCCESS;
	if (builtin_assign_addexist(envlst, arg, var) != FUNCT_SUCCESS)
	{
		if (builtin_assign_addnew(envlst, var) != FUNCT_SUCCESS)
		{
			ft_printf("assign: failed to allocate enough memory\n");
			*exit_code = EXIT_FAILURE;
		}
	}
	ft_strdel(&var);
}
