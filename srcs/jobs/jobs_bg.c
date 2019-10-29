/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   jobs_bg.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: rkuijper <rkuijper@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/10/18 16:46:22 by rkuijper       #+#    #+#                */
/*   Updated: 2019/10/29 11:18:47 by rkuijper      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"
#include <errno.h>
#include <signal.h>

void	jobs_bg_job(t_job *job, bool job_continued)
{
	jobs_print_job_info(job, JOB_OPT_L, g_data->jobs->joblist);
	job->bg = true;
	if (job_continued && kill(-job->pgid, SIGCONT) < 0)
		ft_eprintf("kill (SIGCONT): %e\n", errno);
}
