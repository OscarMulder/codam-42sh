/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   param_to_env.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: jbrinksm <jbrinksm@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/04/03 18:45:30 by jbrinksm       #+#    #+#                */
/*   Updated: 2019/04/25 15:07:39 by mavan-he      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"

char		*param_to_env(char *parameter, char **vshenviron)
{
	int		param_len;
	int		env_index;
	int		index;

	param_len = ft_strlen(parameter);
	env_index = 0;
	while (vshenviron[env_index] != NULL)
	{
		if (ft_strncmp(parameter, vshenviron[env_index], param_len) == 0 &&
			vshenviron[env_index][param_len] == '=')
		{
			index = 0;
			while (vshenviron[env_index][index] != '=')
				index++;
			return (&vshenviron[env_index][index + 1]);
		}
		env_index++;
	}
	return (NULL);
}
