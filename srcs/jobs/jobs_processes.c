/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   jobs_processes.c                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: rkuijper <rkuijper@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/10/21 11:41:03 by rkuijper       #+#    #+#                */
/*   Updated: 2019/10/22 15:37:16 by rkuijper      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"

t_proc		*jobs_new_proc(pid_t pid)
{
	t_proc *proc;

	proc = (t_proc*)ft_memalloc(sizeof(t_proc));
	proc->pid = pid;
	proc->state = PROC_RUNNING;
	return (proc);
}

int			jobs_add_process(t_job *job, pid_t pid)
{
	t_proc *proc;

	proc = jobs_new_proc(pid);
	if (proc == NULL)
		return (FUNCT_ERROR);
	if (job->processes == NULL)
		job->processes = proc;
	else
	{
		proc->next = job->processes;
		job->processes = proc;
	}
	return (FUNCT_SUCCESS);
}
