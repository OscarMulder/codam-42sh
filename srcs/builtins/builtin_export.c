/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   builtin_export.c                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: jbrinksm <jbrinksm@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/06/05 10:33:08 by jbrinksm       #+#    #+#                */
/*   Updated: 2019/06/06 00:14:12 by jbrinksm      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"

void	builtin_export_noargs(t_envlst *envlst, int *exit_code)
{
	t_envlst	*probe;

	probe = envlst;
	while (probe != NULL)
	{
		#ifdef DEBUG
		if (probe->var == NULL)
		{
			ft_printf("builtin_export: error envlst->var == NULL !!!\n");
			*exit_code = EXIT_FAILURE;
			return ;
		}
		#endif
		ft_putendl(probe->var);
		probe = probe->next;
	}
	*exit_code = EXIT_SUCCESS;
}

/*
**	Not sure if we want to create a new variable if the one to be
**	exported doesn't exist yet. Currently now it will do nothing but
**	set exit_code to EXIT_FAILURE.
*/

void	builtin_export_var(char *varname, t_envlst *envlst, int *exit_code)
{
	t_envlst	*probe;
	int			varlen;
	char		*match;

	probe = envlst;
	varlen = ft_strlen(varname);
	match = ft_strjoinchr(varname, '=');
	if (match == NULL)
	{
		*exit_code = EXIT_FAILURE;
		return ;
	}
	while (probe != NULL)
	{
		if (ft_strncmp(match, probe->var, varlen + 1) == 0)
		{
			probe->type = ENV_EXTERN;
			ft_strdel(&match);
			return ;
		}
		probe = probe->next;
	}
	ft_strdel(&match);
	*exit_code = EXIT_FAILURE;
}

void	builtin_export(char **args, t_envlst *envlst, int *exit_code)
{
	int		i;

	i = 1;
	if (args == NULL /* should be redundant --> */|| args[0] == NULL)
		*exit_code = EXIT_FAILURE;
	else if (args[1] == NULL)
		builtin_export_noargs(envlst, exit_code);
	else
	{
		*exit_code = EXIT_SUCCESS;
		while (args[i] != NULL)
		{
			builtin_export_var(args[i], envlst, exit_code);
			i++;
		}
	}
}