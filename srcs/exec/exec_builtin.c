/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   exec_builtin.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: omulder <omulder@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/05/29 16:59:41 by omulder        #+#    #+#                */
/*   Updated: 2019/10/30 11:33:31 by jbrinksm      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"

bool			exec_builtin(char **args, t_vshdata *data)
{
	if (ft_strequ(args[0], "echo") || ft_strequ(args[0], "exit") ||
		ft_strequ(args[0], "cd") || ft_strequ(args[0], "fc") ||
		ft_strequ(args[0], "export") || ft_strequ(args[0], "set") ||
		ft_strequ(args[0], "unset") || ft_strequ(args[0], "history") ||
		ft_strequ(args[0], "type") || ft_strequ(args[0], "alias") ||
		ft_strequ(args[0], "unalias") || ft_strequ(args[0], "jobs") ||
		ft_strequ(args[0], "fg") || ft_strequ(args[0], "bg") ||
		ft_strequ(args[0], "hash"))
	{
		data->jobs->active_job->last_proc->is_builtin = true;
		data->jobs->active_job->last_proc->redir_node = data->current_redirs;
		return (true);
	}
	return (false);
}
