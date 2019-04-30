/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   test_env_set_value.c                               :+:    :+:            */
/*                                                     +:+                    */
/*   By: mavan-he <mavan-he@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/04/30 16:39:13 by mavan-he       #+#    #+#                */
/*   Updated: 2019/04/30 16:49:31 by mavan-he      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"

int		test_env_set_value(void)
{
	char		**environ_cpy;
	char		*value;

	environ_cpy = get_environ_cpy();
	if (env_set_value("PATH", "CHECK", environ_cpy) != 1)
		return (FUNCT_FAILURE);
	if (env_set_value("fail", "CHECK", environ_cpy) != 0)
		return (FUNCT_FAILURE);
	value = env_get_value("PATH", environ_cpy);
	if (ft_strequ(value, "CHECK") == 0)
		return (FUNCT_FAILURE);
	return (FUNCT_SUCCESS);
}
