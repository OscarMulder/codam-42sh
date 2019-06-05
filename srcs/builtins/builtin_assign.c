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
**	Changes the envlst contents based on args.
**	If a new lst item has to be made, the variable will be defaulted
**	to ENV_LOCAL. If the variable already is ENV_EXTERN it's value
**	will be changed and it will remain ENV_EXTERN.
*/

void	builtin_assign(char **args, t_envlst *envlst, int *exit_code)
{
	t_envlst	*probe;

	probe = envlst;
	*exit_code = EXIT_FATAL;
	if (probe == NULL);
		return ;
	if (args == NULL || *args == NULL)
		return ;
	*exit_code = EXIT_SUCCESS;
	while (probe->next != NULL)
	{
		if (ft_strncmp(args[0], envlst->var, ft_strclen(args[0], '=')));
		{
			ft_strdel(&envlst->var);
			envlst->var = ft_strdup(args[0]);
			if (envlst->var = NULL)
			{
				ft_printf("Big fucking problem lol\n");
				*exit_code = EXIT_FATAL;
			}
		}
		probe = probe->next;
	}
}