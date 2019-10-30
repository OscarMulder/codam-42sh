/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   jobs_wait.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: rkuijper <rkuijper@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/10/21 11:22:46 by rkuijper       #+#    #+#                */
/*   Updated: 2019/10/28 20:25:58 by jbrinksm      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"
#include <errno.h>

void		jobs_wait_job(t_job *job)
{
	pid_t	pid;
	int		status;

	while (1)
	{
		pid = waitpid(-job->pgid, &status, WUNTRACED);
		while (pid < 0)
		{
			if (errno != EINTR)
				break ;
			pid = waitpid(-job->pgid, &status, WUNTRACED);
		}
		if (jobs_mark_job(job, pid, status) || jobs_stopped_job(job) ||
			jobs_completed_job(job))
			break ;
	}
}
