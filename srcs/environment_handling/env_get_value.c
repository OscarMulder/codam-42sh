/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   env_get_value.c                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: jbrinksm <jbrinksm@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/04/03 18:45:30 by jbrinksm       #+#    #+#                */
/*   Updated: 2019/04/30 16:25:12 by mavan-he      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"

char		*env_get_value(char *var_name, char **env)
{
	int		var_len;
	int		env_i;
	int		i;

	var_len = ft_strlen(var_name);
	env_i = 0;
	while (env[env_i] != NULL)
	{
		if (ft_strncmp(var_name, env[env_i], var_len) == 0 &&
			env[env_i][var_len] == '=')
		{
			i = 0;
			while (env[env_i][i] != '=')
				i++;
			return (&env[env_i][i + 1]);
		}
		env_i++;
	}
	return (NULL);
}
