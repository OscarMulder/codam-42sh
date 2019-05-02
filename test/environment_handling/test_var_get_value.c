/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   test_var_get_value.c                               :+:    :+:            */
/*                                                     +:+                    */
/*   By: jbrinksm <jbrinksm@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/04/19 18:41:23 by jbrinksm       #+#    #+#                */
/*   Updated: 2019/05/02 10:26:33 by rkuijper      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"

int		test_var_get_value(void)
{
	char		**environ_cpy;
	char		*buf;

	environ_cpy = get_environ_cpy();
	buf = var_get_value("PATH", environ_cpy);
	if (buf == NULL)
		return (FUNCT_FAILURE);
	if (ft_strcmp(buf, getenv("PATH")))
	{
		ft_strdel(&buf);
		return (FUNCT_FAILURE);
	}
	buf = var_get_value("NO_EXIST", environ_cpy);
	if (buf)
	{
		ft_strdel(&buf);
		return (FUNCT_FAILURE);
	}
	return (FUNCT_SUCCESS);
}
