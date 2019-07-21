/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   tools_is_posnumstr.c                               :+:    :+:            */
/*                                                     +:+                    */
/*   By: jbrinksm <jbrinksm@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/07/21 20:18:16 by jbrinksm       #+#    #+#                */
/*   Updated: 2019/07/21 21:18:15 by jbrinksm      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"

bool	tools_is_fdnumstr(char *str)
{
	int	i;

	if (str == NULL || (*str < '0' || *str > '9'))
		return (false);
	i = 0;
	while (str[i] != '\0')
	{
		if (i > 6)
			return (false);
		if (str[i] < '0' || str[i] > '9')
			return (false);
		i++;
	}
	return (true);
}
