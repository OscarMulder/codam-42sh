/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   builtin_exit.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: jbrinksm <jbrinksm@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/04/11 20:15:24 by jbrinksm       #+#    #+#                */
/*   Updated: 2019/05/29 16:50:11 by omulder       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"

void	builtin_exit(char **args)
{
	ft_printf("exit\n");
	if (args[1] != 0 && args[2] == 0)
	{
		exit(ft_atoi(args[1]));
	}
	else if (args[1] != 0 && args[2] != 0)
	{
		ft_printf("Sh3llN4m3: 3x1t: T00 M4ny ArGum3nts\n");
		exit(-1);
	}
	exit(PROG_SUCCESS);
}
