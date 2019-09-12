/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   jobs_job_utils.c                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: omulder <omulder@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/05/31 10:47:19 by tde-jong       #+#    #+#                */
/*   Updated: 2019/08/22 11:25:02 by omulder       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <vsh.h>

int		jobs_get_job_state(t_job *job)
{
	int		status;
	pid_t	result;

	result = waitpid(job->process_id, &status, WNOHANG);
	if (WIFSTOPPED(status))
		return (JOB_SUSPEND);
	if (WIFCONTINUED(status))
		return (JOB_RUNNING);
	if (result != 0)
		return (JOB_EXIT);
	return (JOB_RUNNING);
}
