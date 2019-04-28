/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   builtin_set.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: jbrinksm <jbrinksm@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/03/28 16:47:15 by jbrinksm       #+#    #+#                */
/*   Updated: 2019/04/28 13:31:02 by mavan-he      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"

/*
** WIP
**
** set with no args lists intern variable lst
** set can also be called like set [name] [value]
*/

int			builtin_set(char **args, char **env)
{
	size_t i;

	if (args[1] != NULL)
	{
		ft_putstr("set: too many arguments\n"); // set this to error output
		return (FUNCT_ERROR);
	}
	i = 0;
	while (env[i])
	{
		ft_putendl(env[i]);
		i++;
	}
	return (FUNCT_SUCCESS);
}

/* void	builtin_setenv(char **args, char ***env)
{
	int	argc;

	if (args[1] == NULL)
	// What happens when no args are given
	else if (args[2] == NULL)
	// set NAME to NULL
	// or if NAME does not exists create NAME and set to NULL
	else if (args[3] != NULL)
	{
		ft_putendl_fd("Too many arguments", 2);
		return (FUNCT_FAILURE);
	}
	// return error TOO many arguments
	else
	// set NAME to VALUE
	// or if NAME does not exists create NAME and set to VALUE
	return (FUNCT_SUCCESS)
}
 */






/* 	argc = vsh_count_args(command_row);
	if (argc == 1)
		vsh_env_list(*vshenviron);
	else if (argc == 2 || argc == 3)
		vsh_setenv_add(command_row[1], command_row[2], vshenviron);
	else
		vsh_error(E_TOOMNYARGS, command_row[0]);
} */
/* int		vsh_setenv_checkinput(char *variable, char *value)
{
	int index;

	index = 0;
	while (variable[index] != '\0')
	{
		if (!ft_isalnum(variable[index]) && variable[index] != '_')
			return (FUNCT_FAILURE);
		if (variable[index] == '=')
			return (FUNCT_FAILURE);
		index++;
	}
	index = 0;
	if (!value)
		return (FUNCT_SUCCESS);
	while (value[index] != '\0')
	{
		if (value[index] == '=')
			return (FUNCT_FAILURE);
		index++;
	}
	return (FUNCT_SUCCESS);
}

int		vsh_setenv_add(char *variable, char *value, char ***vshenviron)
{
	char	*input;
	char	**new_vshenviron;
	int		index;

	if (!vsh_setenv_checkinput(variable, value))
	{
		vsh_error(E_SENOTALNUM, "setenv");
		return (FUNCT_FAILURE);
	}
	input = ft_strjoinchr(variable, '=', value);
	index = vsh_findvar(variable, *vshenviron);
	if (index != -1)
	{
		ft_strdel(&(*vshenviron)[index]);
		(*vshenviron)[index] = ft_strdup(input);
	}
	else
	{
		new_vshenviron = ft_arrayaddstr(*vshenviron, input);
		vsh_ft_freearray(vshenviron);
		*vshenviron = new_vshenviron;
	}
	ft_strdel(&input);
	return (FUNCT_SUCCESS);
}

 */