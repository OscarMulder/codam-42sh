/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   jobs_cont.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: rkuijper <rkuijper@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/10/18 16:47:14 by rkuijper       #+#    #+#                */
/*   Updated: 2019/10/21 14:29:39 by rkuijper      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"

void		jobs_continue_job(t_job *job, bool fg)
{
	t_proc *proc;

	proc = job->processes;
	while (proc != NULL)
	{
		if (proc->state == PROC_STOPPED)
			proc->state = PROC_CONTINUED;
		proc = proc->next;
	}
	job->current = builtin_jobs_new_current_val(g_data->jobs->joblist);
	jobs_print_job_info(job, JOB_OPT_P, g_data->jobs->joblist);
	if (fg == true)
		jobs_fg_job(job, true);
	else
		jobs_bg_job(job, true);
}