/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   env_set_value.c                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: jbrinksm <jbrinksm@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/04/03 18:45:30 by jbrinksm       #+#    #+#                */
/*   Updated: 2019/04/30 16:24:25 by mavan-he      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"

int		env_set_value(char *var_name, char *value, char **env)
{
	int		var_len;
	int		env_i;

	var_len = ft_strlen(var_name);
	env_i = 0;
	while (env[env_i] != NULL)
	{
		if (ft_strncmp(var_name, env[env_i], var_len) == 0 &&
			env[env_i][var_len] == '=')
		{
			free(env[env_i]);
			env[env_i] = env_join_name_value(var_name, value);
			if (env[env_i] == NULL)
				return (FUNCT_ERROR); // add error message here ??
			return (FUNCT_SUCCESS);
		}
		env_i++;
	}
	return (FUNCT_FAILURE);
}
