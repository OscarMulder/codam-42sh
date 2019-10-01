/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   builtin_env.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: omulder <omulder@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/10/01 15:34:13 by omulder        #+#    #+#                */
/*   Updated: 2019/10/01 15:36:09 by omulder       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"

void	builtin_env(char **args, t_envlst *envlst)
{
	if (args[1] != NULL)
	{
		ft_eprintf(E_N_TOO_MANY, "env");
		ft_eprintf(U_SET);
		g_state->exit_code = EXIT_WRONG_USE;
		return ;
	}
	while (envlst != NULL)
	{
		if (envlst->type & ENV_EXTERN)
			ft_putendl(envlst->var);
		envlst = envlst->next;
	}
}
