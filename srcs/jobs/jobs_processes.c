/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   jobs_processes.c                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: rkuijper <rkuijper@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/10/21 11:41:03 by rkuijper       #+#    #+#                */
/*   Updated: 2019/10/28 17:51:49 by jbrinksm      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"

t_proc		*jobs_new_proc(void)
{
	t_proc *proc;

	proc = (t_proc*)ft_memalloc(sizeof(t_proc));
	proc->state = PROC_RUNNING;
	return (proc);
}

int			jobs_add_process(t_job *job)
{
	t_proc *proc;
	t_proc *probe;

	proc = jobs_new_proc();
	if (proc == NULL)
		return (FUNCT_ERROR);
	if (job->processes == NULL)
		job->processes = proc;
	else
	{
		probe = job->processes;
		while (probe->next != NULL)
			probe = probe->next;
		probe->next = proc;
	}
	job->last_proc = proc;
	return (FUNCT_SUCCESS);
}
