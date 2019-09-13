/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   jobs_job_utils.c                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: omulder <omulder@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/05/31 10:47:19 by tde-jong       #+#    #+#                */
/*   Updated: 2019/09/13 16:24:21 by jbrinksm      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <vsh.h>

int		tools_get_pid_state(pid_t pid)
{
	int		status;
	pid_t	result;

	result = waitpid(pid, &status, WNOHANG);
	if (WIFSTOPPED(status))
		return (PID_SUSPEND);
	if (WIFCONTINUED(status))
		return (PID_RUNNING);
	if (result != 0)
		return (PID_EXIT);
	return (PID_RUNNING);
}

void	job_set_active(int job_id)
{
	(void)job_id;
}
