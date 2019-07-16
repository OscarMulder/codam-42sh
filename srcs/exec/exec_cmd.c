/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   exec_cmd.c                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: omulder <omulder@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/05/29 17:17:48 by omulder        #+#    #+#                */
/*   Updated: 2019/07/16 19:20:54 by jbrinksm      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"

void		exec_cmd(char **args, t_envlst *envlst, int *exit_code, int pipeside, int *currentpipe, int *parentpipe, t_stdfds fds)
{
	if (exec_builtin(args, envlst, exit_code, pipeside, currentpipe, parentpipe, fds) == false)
	{
		if (exec_external(args, envlst, exit_code, pipeside, currentpipe, parentpipe, fds) == false)
		{
			ft_printf("%s: Command not found.\n", args[0]);
			*exit_code = EXIT_NOTFOUND;
		}
	}
	ft_strarrdel(&args);
}
