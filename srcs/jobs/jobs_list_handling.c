/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   jobs_list_handling.c                               :+:    :+:            */
/*                                                     +:+                    */
/*   By: omulder <omulder@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/05/31 10:47:19 by tde-jong       #+#    #+#                */
/*   Updated: 2019/10/30 10:48:53 by rkuijper      ########   odam.nl         */
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

t_job			*jobs_remove_job(t_job *joblist, pid_t pid)
{
	t_job *tmp;

	if (joblist == NULL)
		return (NULL);
	if (joblist->pgid == pid)
	{
		tmp = joblist->next;
		ft_strdel(&joblist->command);
		ft_memdel((void**)&joblist);
		return (tmp);
	}
	joblist->next = jobs_remove_job(joblist->next, pid);
	return (joblist);
}

t_job				*jobs_add_job(t_vshdata *data, t_job *job)
{
	int		jid;
	t_job	*tmp;

	jid = 1;
	if (data->jobs->joblist == NULL)
		data->jobs->joblist = job;
	else
	{
		tmp = data->jobs->joblist;
		jid = tmp->job_id;
		while (tmp->next)
		{
			tmp = tmp->next;
			if (tmp->job_id > jid)
				jid = tmp->job_id;
		}
		job->next = job;
	}
	job->job_id = jid;
	job->current = builtin_jobs_new_current_val(data->jobs->joblist);
	return (job);
}

t_job				*jobs_last_child(t_job *job)
{
	t_job *tmp;

	if (job == NULL)
		return (NULL);
	tmp = job;
	while (tmp->child)
		tmp = tmp->child;
	return (tmp);
}
