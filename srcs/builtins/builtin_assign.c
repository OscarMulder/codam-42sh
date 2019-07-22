/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   builtin_assign.c                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: jbrinksm <jbrinksm@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/06/05 09:09:49 by jbrinksm       #+#    #+#                */
/*   Updated: 2019/07/22 10:51:08 by mavan-he      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"

int			builtin_assign_addexist(t_envlst *envlst, char *arg,
		char *var, int env_type)
{
	t_envlst	*probe;
	int			varlen;

	probe = envlst;
	varlen = ft_strclen(arg, '=');
	while (probe != NULL)
	{
		if (ft_strnequ(arg, probe->var, varlen) == true &&
		probe->var[varlen] == '=')
		{
			ft_strdel(&probe->var);
			probe->type = env_type;
			probe->var = var;
			return (FUNCT_SUCCESS);
		}
		probe = probe->next;
	}
	return (FUNCT_FAILURE);
}

int			builtin_assign_addnew(t_envlst *envlst, char *var, int env_type)
{
	t_envlst	*newitem;

	newitem = env_lstnew(var, env_type);
	ft_strdel(&var);
	if (newitem == NULL)
		return (FUNCT_ERROR);
	env_lstadd_to_sortlst(envlst, newitem);
	return (FUNCT_SUCCESS);
}

/*
**	NOT SURE IF CORRECT ASSUMPTIONS AS OF HOW IT IS SUPPOSED TO WORK:
**	Changes the envlst contents based on arg.
**	If a new lst item has to be made, the variable will be defaulted
**	to ENV_LOCAL. If the variable already is ENV_EXTERN its value
**	will be changed and it will remain ENV_EXTERN.
*/

void		builtin_assign(char *arg, t_envlst *envlst,
	int *exit_code, int env_type)
{
	char		*var;

	*exit_code = EXIT_FAILURE;
	if (envlst == NULL || arg == NULL)
		return ;
	var = ft_strdup(arg);
	if (var == NULL)
		return ;
	if (tool_check_for_whitespace(arg) == true)
		env_type |= ENV_WHITESPACE;
	else
		env_type &= ~ENV_WHITESPACE;
	*exit_code = EXIT_SUCCESS;
	if (builtin_assign_addexist(envlst, arg, var, env_type) != FUNCT_SUCCESS)
	{
		if (builtin_assign_addnew(envlst, var, env_type) != FUNCT_SUCCESS)
		{
			ft_printf("assign: failed to allocate enough memory\n");
			*exit_code = EXIT_FAILURE;
		}
	}
}
