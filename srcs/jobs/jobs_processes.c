/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   jobs_processes.c                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: rkuijper <rkuijper@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/10/21 11:41:03 by rkuijper       #+#    #+#                */
/*   Updated: 2019/10/30 14:15:39 by rkuijper      ########   odam.nl         */
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

void		jobs_flush_process(t_proc *proc)
{
	t_proc *tmp;

	while (proc != NULL)
	{
		tmp = proc->next;
		if (proc->binary != NULL)
			ft_strdel(&proc->binary);
		if (proc->argv != NULL)
			ft_strarrdel(&proc->argv);
		// TODO: Reimplement when environment variables are working correctly.
		// if (proc->env != NULL)
		// 	ft_strarrdel(&proc->env);
		if (proc->env != NULL)
			ft_memdel((void**)&proc->env);
		ft_memdel((void**)&proc);
		proc = tmp;
	}
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
