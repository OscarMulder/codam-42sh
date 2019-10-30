/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   exec_cmd.c                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: omulder <omulder@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/05/29 17:17:48 by omulder        #+#    #+#                */
/*   Updated: 2019/10/30 13:23:03 by rkuijper      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"

void		exec_cmd(char **args, t_vshdata *data)
{
	t_job *job;

	job = jobs_last_child(data->jobs->active_job);
	if (job == NULL)
		return ;
	jobs_update_job_command(job, args);
	jobs_add_process(job);
	if (job->last_proc == NULL)
		exit(1);
	job->last_proc->argv = args;
	if (exec_builtin(args, data) == false)
		exec_external(args, data);
	env_remove_tmp(data->envlst);
}
