/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   jobs_status.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: rkuijper <rkuijper@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/10/21 11:25:18 by rkuijper       #+#    #+#                */
/*   Updated: 2019/10/22 13:59:12 by rkuijper      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"

int			jobs_stopped_job(t_job *job)
{
	int		s;
	t_proc	*proc;

	s = 0;
	proc = job->processes;
	while (proc != NULL)
	{
		if (proc->state != PROC_COMPLETED)
		{
			if (proc->state != PROC_STOPPED)
				return (FUNCT_FAILURE);
			s++;
		}
		proc = proc->next;
	}
	return (s > 0);
}

int			jobs_completed_job(t_job *job)
{
	t_proc *proc;

	proc = job->processes;
	while (proc != NULL)
	{
		if (proc->state != PROC_COMPLETED)
			return (FUNCT_FAILURE);
		proc = proc->next;
	}
	return (FUNCT_SUCCESS);
}
