/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   builtin_setenv.c                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: mavan-he <mavan-he@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/04/25 17:17:59 by mavan-he       #+#    #+#                */
/*   Updated: 2019/04/25 17:40:35 by mavan-he      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

/*
** WORK IN PROGRESS
*/

#include "vsh.h"

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

void	vsh_setenv(char **command_row, char ***vshenviron)
{
	int	argc;

	argc = vsh_count_args(command_row);
	if (argc == 1)
		vsh_env_list(*vshenviron);
	else if (argc == 2 || argc == 3)
		vsh_setenv_add(command_row[1], command_row[2], vshenviron);
	else
		vsh_error(E_TOOMNYARGS, command_row[0]);
}
 */