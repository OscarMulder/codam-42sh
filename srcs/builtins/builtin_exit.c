/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   builtin_exit.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: omulder <omulder@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/04/11 20:15:24 by jbrinksm       #+#    #+#                */
/*   Updated: 2019/09/03 17:34:28 by mavan-he      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"

static void	reset_exit(int exit_code, t_vshdataterm *term_p)
{
	if (g_state->shell_type == SHELL_NORMAL &&
		term_reset(term_p) == FUNCT_FAILURE)
		ft_eprintf(E_NOT_RESET);
	exit(exit_code);
}

void		builtin_exit(char **args, t_vshdata *data)
{
	ft_eprintf("exit\n");
	history_to_file(data);
	if (args == NULL)
		reset_exit(g_state->exit_code, data->term);
	if (args[1] != NULL && args[2] == NULL)
	{
		if (ft_aisint(args[1]) == true)
			reset_exit(ft_atoi(args[1]), data->term);
		ft_eprintf(E_N_P_NUM_REQ, "exit", args[1]);
		reset_exit(EXIT_WRONG_USE, data->term);
	}
	else if (args[1] != NULL && args[2] != NULL)
	{
		ft_eprintf(E_N_TOO_MANY, "exit");
		g_state->exit_code = EXIT_WRONG_USE;
		return ;
	}
	reset_exit(g_state->exit_code, data->term);
}
