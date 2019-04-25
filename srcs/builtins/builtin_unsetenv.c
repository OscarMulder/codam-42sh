/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   builtin_unsetenv.c                                 :+:    :+:            */
/*                                                     +:+                    */
/*   By: jbrinksm <jbrinksm@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/03/28 21:49:00 by jbrinksm       #+#    #+#                */
/*   Updated: 2019/04/25 17:40:44 by mavan-he      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

/*
** WORK IN PROGRESS
*/

#include "vsh.h"

/* int		vsh_unsetenv_checkinput(char *variable)
{
	int index;

	index = 0;
	while (variable[index] != '\0')
	{
		if (!ft_isalnum(variable[index]) && variable[index] != '_')
			return (FUNCT_FAILURE);
		index++;
	}
	index = 0;
	return (FUNCT_SUCCESS);
}

int		vsh_unsetenv_remove(char ***vshenviron, int index)
{
	ft_strdel(&(*vshenviron)[index]);
	while ((*vshenviron)[index + 1] != NULL)
	{
		(*vshenviron)[index] = (*vshenviron)[index + 1];
		index++;
	}
	(*vshenviron)[index] = NULL;
	return (FUNCT_SUCCESS);
}

int		vsh_unsetenv_find(char *variable, char ***vshenviron)
{
	int		index;
	char	*varname;
	int		remove_succesful;

	index = 0;
	remove_succesful = 0;
	if (!vsh_unsetenv_checkinput(variable))
	{
		vsh_error(E_UENOTALNUM, "unsetenv");
		return (FUNCT_FAILURE);
	}
	while ((*vshenviron)[index] != NULL)
	{
		varname = ft_strcdup((*vshenviron)[index], '=');
		if (!ft_strcmp(varname, variable))
		{
			ft_strdel(&varname);
			remove_succesful += vsh_unsetenv_remove(vshenviron, index);
		}
		ft_strdel(&varname);
		index++;
	}
	if (remove_succesful)
		return (FUNCT_SUCCESS);
	return (FUNCT_FAILURE);
}

void	vsh_unsetenv(char **command_row, char ***vshenviron)
{
	int	argc;

	argc = vsh_count_args(command_row);
	if (argc == 1)
		vsh_error(E_INVARGS, command_row[0]);
	else if (argc == 2)
		vsh_unsetenv_find(command_row[1], vshenviron);
	else
		vsh_error(E_TOOMNYARGS, command_row[0]);
}
 */