/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   builtin_env.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: jbrinksm <jbrinksm@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/03/28 16:47:15 by jbrinksm       #+#    #+#                */
/*   Updated: 2019/04/25 17:40:14 by mavan-he      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

/*
** WORK IN PROGRESS
*/

#include "vsh.h"

int			builtin_env(char **args, char **env)
{
	size_t i;

	if (args[1])
	{
		ft_putendl_fd("Too many arguments", 2);
		return (FUNCT_FAILURE);
	}
	i = 0;
	while (env[i])
	{
		ft_putendl(env[i]);
		i++;
	}
	return (FUNCT_SUCCESS);
}

/*
void		vsh_env_refresh(char **command_row, char ***vshenviron)
{
	if (!ft_strcmp(command_row[1], "REFRESH"))
	{
		vsh_ft_freearray(vshenviron);
		*vshenviron = vsh_create_vshenviron();
	}
	else
		vsh_error(E_INVARGS, command_row[0]);
}

void		vsh_env_list(char **vshenviron)
{
	int i;

	i = 0;
	while (vshenviron[i])
	{
		ft_printf("%s\n", vshenviron[i]);
		i++;
	}
}

void		vsh_env(char **command_row, char ***vshenviron)
{
	int	argc;

	argc = vsh_count_args(command_row);
	if (argc == 1)
		vsh_env_list(*vshenviron);
	else if (argc == 2)
		vsh_env_refresh(command_row, vshenviron);
	else
		vsh_error(E_TOOMNYARGS, command_row[0]);
}
 */