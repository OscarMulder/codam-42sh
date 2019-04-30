/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   env_join_name_value.c                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: mavan-he <mavan-he@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/04/30 16:00:15 by mavan-he       #+#    #+#                */
/*   Updated: 2019/04/30 16:37:08 by mavan-he      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"

char	*env_join_name_value(char *name, char *value)
{
	char *str;

	str = ft_strnew(ft_strlen(name) + ft_strlen(value) + 1);
	if (str == NULL)
		return (NULL);
	ft_strcpy(str, name);
	ft_strcat(str, "=");
	ft_strcat(str, value);
	return (str);
}
