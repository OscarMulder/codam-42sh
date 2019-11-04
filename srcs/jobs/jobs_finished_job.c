/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   jobs_finished_job.c                                :+:    :+:            */
/*                                                     +:+                    */
/*   By: rkuijper <rkuijper@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/10/31 09:39:34 by rkuijper       #+#    #+#                */
/*   Updated: 2019/11/04 10:50:11 by rkuijper      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"

void		jobs_finished_job(t_job *job)
{
	t_job *child;

	if (job == NULL)
		return ;
	g_state->exit_code = job->last_proc->exit_status;
	jobs_remove_job(&g_data->jobs->joblist, job->pgid);
	child = job->child;
	if (child != NULL)
	{
		if ((job->andor == ANDOR_NONE ||
			(job->andor == ANDOR_AND && g_state->exit_code == 0) ||
			(job->andor == ANDOR_OR && g_state->exit_code != 0)))
		{
			jobs_flush_job(job, false);
			jobs_launch_job(child);
			return ;
		}
		jobs_finished_job(child);
	}
	jobs_flush_job(job, false);
}
