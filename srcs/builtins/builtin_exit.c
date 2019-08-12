/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   builtin_exit.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: omulder <omulder@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/04/11 20:15:24 by jbrinksm       #+#    #+#                */
/*   Updated: 2019/08/12 10:34:25 by tde-jong      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"

void	builtin_exit(char **args, t_vshdata *vshdata)
{
	ft_printf("exit\n");
	history_to_file(vshdata);
	if (args == NULL)
		exit(g_state->exit_code);
	if (args[1] != NULL && args[2] == NULL)
	{
		if (ft_aisint(args[1]) == true)
			exit(ft_atoi(args[1]));
		ft_printf("vsh: exit: %s: numeric argument required\n", args[1]);
		exit(EXIT_WRONG_USE);
	}
	else if (args[1] != NULL && args[2] != NULL)
	{
		ft_eprintf("vsh: exit: too many arguments\n");
		g_state->exit_code = EXIT_WRONG_USE;
		return ;
	}
	exit(g_state->exit_code);
}
