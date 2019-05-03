/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   builtin_set.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: mavan-he <mavan-he@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/05/03 17:50:50 by mavan-he       #+#    #+#                */
/*   Updated: 2019/05/03 18:02:32 by mavan-he      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"

/*
** >>>>>>>>>>> WIP <<<<<<<<<<<<<
** SET:
**
** set lists both internal and external environment
**
** TODO:
** sorting both lists together ?
*/

int			builtin_set(char **args, char **var_intern, char **var_extern)
{
	int i;

	if (args[1] != NULL)
	{
		ft_putendl_fd("set: too many arguments", 2);
		return (FUNCT_FAILURE);
	}
	i = 0;
	while (var_extern[i])
	{
		ft_putendl(var_intern[i]);
		i++;
	}
	i = 0;
	while (var_intern[i])
	{
		ft_putendl(var_intern[i]);
		i++;
	}
	return (FUNCT_SUCCESS);
}