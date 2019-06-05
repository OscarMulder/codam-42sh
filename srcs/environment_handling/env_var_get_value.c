/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   env_var_get_value.c                                :+:    :+:            */
/*                                                     +:+                    */
/*   By: jbrinksm <jbrinksm@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/04/03 18:45:30 by jbrinksm       #+#    #+#                */
/*   Updated: 2019/06/05 09:16:18 by tde-jong      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"

char		*env_var_get_value(char *var_key, char **vararray)
{
	int		var_len;
	int		env_i;

	var_len = ft_strlen(var_key);
	env_i = 0;
	while (vararray[env_i] != NULL)
	{
		if (ft_strnequ(var_key, vararray[env_i], var_len) &&
			vararray[env_i][var_len] == '=')
			return (ft_strchr(vararray[env_i], '=') + 1);
		env_i++;
	}
	return (NULL);
}
