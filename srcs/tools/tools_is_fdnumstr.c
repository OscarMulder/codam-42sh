/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   tools_is_fdnumstr.c                                :+:    :+:            */
/*                                                     +:+                    */
/*   By: jbrinksm <jbrinksm@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/07/21 20:18:16 by jbrinksm       #+#    #+#                */
/*   Updated: 2019/07/23 11:36:07 by jbrinksm      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"

bool	tools_is_fdnumstr(char *str)
{
	int	i;

	if (str == NULL || ft_isdigit(*str) == false)
		return (false);
	i = 0;
	while (str[i] != '\0')
	{
		if (i > 6)
			return (false);
		if (ft_isdigit(str[i]) == false)
			return (false);
		i++;
	}
	return (true);
}
