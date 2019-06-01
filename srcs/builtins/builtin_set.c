/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   builtin_set.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: mavan-he <mavan-he@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/05/03 17:50:50 by mavan-he       #+#    #+#                */
/*   Updated: 2019/06/01 10:50:10 by omulder       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"

int			builtin_set(char **args, char **var_intern, char **var_extern)
{
	int i;

	if (args[1] != NULL)
	{
		ft_putendl_fd("set: too many arguments", 2);
		return (FUNCT_FAILURE);
	}
	ft_putstrarr(var_extern);
	ft_putstrarr(var_intern);
	return (FUNCT_SUCCESS);
}
