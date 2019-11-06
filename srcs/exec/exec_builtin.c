/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   exec_builtin.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: omulder <omulder@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/05/29 16:59:41 by omulder        #+#    #+#                */
/*   Updated: 2019/11/06 13:52:20 by rkuijper      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"

bool			exec_builtin(char **args, t_vshdata *data)
{
	if (tools_is_builtin(args[0]) == true)
	{
		jobs_last_child(data->jobs->active_job)->last_proc->is_builtin = true;
		jobs_last_child(data->jobs->active_job)->last_proc->redir_and_assign =
			data->current_redir_and_assign;
		return (true);
	}
	return (false);
}
