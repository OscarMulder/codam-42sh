/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   param_to_env.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: jbrinksm <jbrinksm@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/04/03 18:45:30 by jbrinksm       #+#    #+#                */
/*   Updated: 2019/04/25 12:52:38 by mavan-he      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"

/*
** add errno here
*/

char		*param_to_env(char *parameter, char **vshenviron)
{
	int		env_index;
	int		index;
	char	*name;
	char	*value;

	env_index = 0;
	while (vshenviron[env_index] != NULL)
	{
		index = 0;
		name = ft_strcdup(vshenviron[env_index], '=');
		if (name == NULL)
			return (NULL);
		if (ft_strcmp(parameter, name) == 0)
		{
			while (vshenviron[env_index][index] != '=')
				index++;
			value = ft_strdup(&(vshenviron[env_index][index + 1]));
			ft_strdel(&name);
			return (value);
		}
		ft_strdel(&name);
		env_index++;
	}
	return (NULL);
}
