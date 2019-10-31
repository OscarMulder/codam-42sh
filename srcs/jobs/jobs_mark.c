/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   jobs_mark.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: rkuijper <rkuijper@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/10/21 11:51:41 by rkuijper       #+#    #+#                */
/*   Updated: 2019/10/31 16:52:54 by rkuijper      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"
#include <signal.h>
#include <string.h>

static int	change_status(t_proc *proc, int status)
{
	proc->exit_status = status;
	if (WIFCONTINUED(status))
		proc->state = PROC_CONTINUED;
	else if (WIFSTOPPED(status))
		proc->state = PROC_STOPPED;
	else
	{
		if (WIFSIGNALED(status))
			ft_eprintf("%d: Terminated by sig%s\n", proc->pid,
				sys_signame[WTERMSIG(status)]);
		proc->state = PROC_COMPLETED;
	}
	return (FUNCT_FAILURE);
}

int			jobs_mark_process_status(pid_t pid, int status)
{
	t_job	*job;
	t_proc	*proc;

	if (pid > 0)
	{
		job = g_data->jobs->joblist;
		while (job != NULL)
		{
			proc = job->processes;
			while (proc != NULL)
			{
				if (proc->pid == pid)
					return (change_status(proc, status));
				proc = proc->next;
			}
			job = job->next;
		}
	}
	return (FUNCT_ERROR);
}
