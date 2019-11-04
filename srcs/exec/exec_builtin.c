/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   exec_builtin.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: omulder <omulder@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/05/29 16:59:41 by omulder        #+#    #+#                */
/*   Updated: 2019/11/04 13:57:38 by jbrinksm      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"

bool			exec_builtin(char **args, t_proc *proc)
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
		proc->is_builtin = true;
		// jobs_last_child(data->jobs->active_job)->last_proc->redir_and_assign =
		// 	data->current_redir_and_assign;
		return (true);
	}
	return (false);
}
