/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   test_env_join_name_value.c                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: mavan-he <mavan-he@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/04/30 16:51:29 by mavan-he       #+#    #+#                */
/*   Updated: 2019/04/30 17:11:19 by mavan-he      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"

int		test_env_join_name_value(void)
{
	if (ft_strequ(env_join_name_value("name", "value"), "name=value") != 1)
		return (FUNCT_FAILURE);
	return (FUNCT_SUCCESS);
}
