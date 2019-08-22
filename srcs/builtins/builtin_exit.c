/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   builtin_exit.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: omulder <omulder@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/04/11 20:15:24 by jbrinksm       #+#    #+#                */
/*   Updated: 2019/08/22 11:04:50 by omulder       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"

static void	reset_exit(int exit_code, t_term *term_p)
{
	if (term_reset(term_p) == FUNCT_FAILURE)
		ft_eprintf(E_NOT_RESET);
	exit(exit_code);
}

void		builtin_exit(char **args, t_vshdata *vshdata)
{
	ft_printf("exit\n");
	history_to_file(vshdata);
	if (args == NULL)
		reset_exit(g_state->exit_code, vshdata->term);
	if (args[1] != NULL && args[2] == NULL)
	{
		if (ft_aisint(args[1]) == true)
			reset_exit(ft_atoi(args[1]), vshdata->term);
		ft_eprintf(E_N_P_NUM_REQ, "exit", args[1]);
		reset_exit(EXIT_WRONG_USE, vshdata->term);
	}
	else if (args[1] != NULL && args[2] != NULL)
	{
		ft_eprintf(E_N_TOO_MANY, "exit");
		g_state->exit_code = EXIT_WRONG_USE;
		return ;
	}
	reset_exit(g_state->exit_code, vshdata->term);
}
