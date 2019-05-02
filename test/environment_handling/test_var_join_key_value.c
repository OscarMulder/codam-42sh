/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   test_var_join_key_value.c                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: mavan-he <mavan-he@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/04/30 16:51:29 by mavan-he       #+#    #+#                */
/*   Updated: 2019/05/02 10:26:12 by rkuijper      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"

int		test_var_join_key_value(void)
{
	if (ft_strequ(var_join_key_value("key", "value"), "key=value") != 1)
		return (FUNCT_FAILURE);
	return (FUNCT_SUCCESS);
}
