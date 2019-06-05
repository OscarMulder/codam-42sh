/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   exec_cmd.c                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: omulder <omulder@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/05/29 17:17:48 by omulder        #+#    #+#                */
/*   Updated: 2019/05/31 11:06:08 by tde-jong      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"

void		exec_cmd(char **args, t_envlst *envlst, int *exit_code)
{
	char	**vshenviron;

	if (exec_builtin(args, envlst, exit_code) == false)
	{
		vshenviron = env_lsttoarr(envlst, ENV_EXTERN);
		if (vshenviron == NULL)
		{
			ft_printf("vsh: failed to allocate enough memory!\n");
			*exit_code = EXIT_FAILURE;
		}
		else if (exec_external(args, vshenviron, exit_code) == false)
		{
			ft_printf("%s: Command not found.\n", args[0]);
			*exit_code = EXIT_NOTFOUND;
		}
	}
	ft_strarrdel(&args);
}
