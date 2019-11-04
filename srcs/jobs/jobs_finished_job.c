/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   jobs_finished_job.c                                :+:    :+:            */
/*                                                     +:+                    */
/*   By: rkuijper <rkuijper@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/10/31 09:39:34 by rkuijper       #+#    #+#                */
/*   Updated: 2019/11/04 16:53:20 by jbrinksm      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"

void		jobs_finished_job(t_job *job)
{
	t_andor		andor;
	t_job		*child;

	if (job == NULL)
		return ;
	jobs_remove_job(&g_data->jobs->joblist, job->pgid);
	child = job->child;
	andor = job->andor;
	jobs_flush_job(job);
	if (child != NULL)
	{
		if ((job->andor == ANDOR_NONE ||
			(job->andor == ANDOR_AND && g_state->exit_code == 0) ||
			(job->andor == ANDOR_OR && g_state->exit_code != 0)))
		{
			jobs_launch_job(child);
			return ;
		}
		jobs_finished_job(child);
	}
	ft_printf(">>><<><><><><><>!!!!!!!!!! killed\n");
}
