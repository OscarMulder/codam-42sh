/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   jobs_mark.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: rkuijper <rkuijper@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/10/21 11:51:41 by rkuijper       #+#    #+#                */
/*   Updated: 2019/10/28 15:48:45 by jbrinksm      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"
#include <errno.h>

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
				{
					proc->exit_status = status;
					if (WIFSTOPPED(status))
						proc->state = PROC_STOPPED;
					else
					{
						proc->state = PROC_COMPLETED;
						if (WIFSIGNALED(status))
							ft_eprintf("%d: Terminated by signal %d\n", proc->pid,
								WTERMSIG(status));
					}
					return (0);
				}
				proc = proc->next;
			}
			job = job->next;
		}
	}
	else if (pid == 0 || errno == ECHILD)
		return (FUNCT_ERROR);
	return (FUNCT_ERROR);
}

int			jobs_mark_proc(t_proc *proc, int status)
{
	if (WIFSTOPPED(status))
	{
		proc->exit_status = 0;
		proc->state = PROC_STOPPED;
	}
	else
	{
		proc->exit_status = status;
		proc->state = PROC_COMPLETED;
		if (WIFEXITED(status))
			proc->exit_status = WEXITSTATUS(status);
		else if (WIFSIGNALED(status) && WTERMSIG(status) != SIGINT)
		{
			if (WTERMSIG(status) != SIGPIPE)
				ft_eprintf("%d: Terminated by signal %d\n", (int)proc->pid,
					WTERMSIG(status));
			proc->exit_status = 1;
		}
		else if (WTERMSIG(status) == SIGINT)
			proc->exit_status = 130;
	}
	return (FUNCT_SUCCESS);
}

int			jobs_mark_job(t_job *job, pid_t pid, int status)
{
	t_proc	*proc;

	if (pid > 0)
	{
		proc = job->processes;
		while (proc)
		{
			if (proc->pid == pid)
				return (jobs_mark_proc(proc, status));
			proc = proc->next;
		}
		return (FUNCT_FAILURE);
	}
	return (FUNCT_FAILURE);
}

int			jobs_mark_pool(pid_t pid, int status)
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
				{
					jobs_mark_proc(proc, status);
					if (job->bg == false && proc->state == PROC_STOPPED)
						job->bg = true;
				}
				proc = proc->next;
			}
			job = job->next;
		}
		return (FUNCT_SUCCESS);
	}
	return (FUNCT_SUCCESS);
}
