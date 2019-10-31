/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   jobs_list_handling.c                               :+:    :+:            */
/*                                                     +:+                    */
/*   By: omulder <omulder@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/05/31 10:47:19 by tde-jong       #+#    #+#                */
/*   Updated: 2019/10/31 09:31:59 by rkuijper      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"

t_job			*jobs_new_job(void)
{
	t_job *new;

	new = (t_job*)ft_memalloc(sizeof(t_job));
	new->bg = false;
	new->command = ft_strnew(1);
	return (new);
}

void			jobs_flush_job(t_job *job)
{
	jobs_flush_process(job->processes);
	if (job->command != NULL)
		ft_strdel(&job->command);
	ft_memdel((void**)&job);
}

t_job			*jobs_remove_job(t_job **joblist, pid_t pgid)
{
	t_job	*job;
	t_job	*tmp;

	if (joblist == NULL || *joblist == NULL)
		return (NULL);
	job = *joblist;
	if (job->pgid == pgid)
	{
		*joblist = job->next;
		job->next = NULL;
		return (job);
	}
	while (job != NULL && job->next != NULL)
	{
		if (job->next->pgid == pgid)
		{
			tmp = job->next;
			job->next = tmp->next;
			tmp->next = NULL;
			return (tmp);
		}
		job = job->next;
	}
	return (NULL);
}

t_job			*jobs_add_job(t_vshdata *data, t_job *job)
{
	int		jid;
	t_job	*tmp;

	jid = 1;
	if (data->jobs->joblist == NULL)
		data->jobs->joblist = job;
	else
	{
		tmp = data->jobs->joblist;
		jid = tmp->job_id + 1;
		while (tmp->next)
		{
			if (tmp->pgid == job->pgid)
				return (job);
			tmp = tmp->next;
			if (tmp->job_id > jid)
				jid = tmp->job_id + 1;
			else
				jid++;
		}
		if (tmp->pgid == job->pgid)
			return (job);
		tmp->next = job;
	}
	job->job_id = jid;
	job->current = builtin_jobs_new_current_val(data->jobs->joblist);
	return (job);
}

t_job			*jobs_last_child(t_job *job)
{
	t_job *tmp;

	if (job == NULL)
		return (NULL);
	tmp = job;
	while (tmp->child)
		tmp = tmp->child;
	return (tmp);
}
