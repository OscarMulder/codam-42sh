/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   exec_builtin.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: omulder <omulder@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/05/29 16:59:41 by omulder        #+#    #+#                */
/*   Updated: 2019/10/22 15:38:20 by rkuijper      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"

static bool		exec_builtin_cont(char **args, t_vshdata *data)
{
	if (ft_strequ(args[0], "history"))
		history_print(data->history->history);
	else if (ft_strequ(args[0], "type"))
		builtin_type(args, data->envlst, data->alias->aliaslst);
	else if (ft_strequ(args[0], "alias"))
		builtin_alias(args, &data->alias->aliaslst);
	else if (ft_strequ(args[0], "unalias"))
		builtin_unalias(args, &data->alias->aliaslst);
	else if (ft_strequ(args[0], "jobs"))
		builtin_jobs(args, data);
	else if (ft_strequ(args[0], "fg"))
		builtin_fg(args, data);
	else if (ft_strequ(args[0], "bg"))
		builtin_bg(args, data);
	else if (ft_strequ(args[0], "hash"))
		builtin_hash(args, data);
	else
		return (false);
	return (true);
}

bool			exec_builtin(char **args, t_vshdata *data)
{
	if (ft_strequ(args[0], "echo"))
		builtin_echo(args);
	else if (ft_strequ(args[0], "exit"))
		builtin_exit(args, data);
	else if (ft_strequ(args[0], "cd"))
		builtin_cd(args, data);
	else if (ft_strequ(args[0], "fc"))
		builtin_fc(args, data);
	else if (ft_strequ(args[0], "export"))
		builtin_export(args, data);
	else if (ft_strequ(args[0], "set"))
		builtin_set(args, data->envlst);
	else if (ft_strequ(args[0], "unset"))
		builtin_unset(args, data->envlst);
	else if (ft_strequ(args[0], "type"))
		builtin_type(args, data->envlst, data->alias->aliaslst);
	else if (ft_strequ(args[0], "alias"))
		builtin_alias(args, &data->alias->aliaslst);
	else if (ft_strequ(args[0], "unalias"))
		builtin_unalias(args, &data->alias->aliaslst);
	else
		return (exec_builtin_cont(args, data));
	return (true);
}
