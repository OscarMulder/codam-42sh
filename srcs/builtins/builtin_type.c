/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   builtin_type.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: tde-jong <tde-jong@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/08/06 13:09:18 by tde-jong       #+#    #+#                */
/*   Updated: 2019/08/06 14:55:39 by tde-jong      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"

static bool	is_builtin(char *name)
{
	return (ft_strequ(name, "echo") || ft_strequ(name, "exit") ||
		ft_strequ(name, "cd") || ft_strequ(name, "export") ||
		ft_strequ(name, "set") || ft_strequ(name, "unset") ||
		ft_strequ(name, "history") || ft_strequ(name, "type") ||
		ft_strequ(name, "alias") || ft_strequ(name, "unalias"));
}

void		builtin_type(char **args, t_envlst *envlst)
{
	size_t	i;
	char	*binary;

	g_state->exit_code = EXIT_SUCCESS;
	i = 1;
	while (args[i] != NULL)
	{
		if (is_builtin(args[i]))
			ft_printf("%s is a shell builtin\n", args[i]);
		else if (exec_find_binary(args[i], envlst, &binary) && binary != NULL)
			ft_printf("%s is %s\n", args[i], binary);
		else
		{
			g_state->exit_code = EXIT_FAILURE;
			ft_eprintf("vsh: type: %s: not found\n", args[i]);
		}
		i++;
	}
}
