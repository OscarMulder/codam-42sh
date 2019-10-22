/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   jobs_list_handling.c                               :+:    :+:            */
/*                                                     +:+                    */
/*   By: omulder <omulder@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/05/31 10:47:19 by tde-jong       #+#    #+#                */
/*   Updated: 2019/10/22 14:43:54 by rkuijper      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"

static t_job	*new_job(pid_t pid, int jid, char *command, int current)
{
	t_job *new;

	new = (t_job*)ft_memalloc(sizeof(t_job));
	new->bg = true;
	new->pgid = pid;
	new->next = NULL;
	new->job_id = jid;
	new->processes = NULL;
	new->current = current;
	new->command = ft_strdup(command);
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

t_job				*jobs_add_job(t_vshdata *data, pid_t pid, char *command)
{
	int		jid;
	t_job	*job;

	if (data->jobs->joblist == NULL)
	{
		data->jobs->joblist = new_job(pid, 1, command,
			builtin_jobs_new_current_val(data->jobs->joblist));
		return (data->jobs->joblist);
	}
	else
	{
		job = data->jobs->joblist;
		jid = job->job_id;
		while (job->next)
		{
			job = job->next;
			if (job->job_id > jid)
				jid = job->job_id;
		}
		job->next = new_job(pid, jid + 1, command,
			builtin_jobs_new_current_val(data->jobs->joblist));
		return (job->next);
	}
}
