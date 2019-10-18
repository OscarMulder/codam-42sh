/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   jobs_list_handling.c                               :+:    :+:            */
/*                                                     +:+                    */
/*   By: omulder <omulder@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/05/31 10:47:19 by tde-jong       #+#    #+#                */
/*   Updated: 2019/10/18 17:06:16 by rkuijper      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"

static t_job	*new_job(pid_t pid, int jid, char *command, int current)
{
	t_job *new;

	new = ft_memalloc(sizeof(t_job));
	new->bg = true;
	new->pgid = pid;
	new->next = NULL;
	new->job_id = jid;
	new->current = current;
	new->command = ft_strdup(command);
	return (new);
}

t_job			*jobs_remove_job(t_job *job, pid_t pid)
{
	t_job *tmp;

	if (job == NULL)
		return (NULL);
	if (job->pgid == pid)
	{
		tmp = job->next;
		ft_strdel(&job->command);
		free(job);
		job = NULL;
		return (tmp);
	}
	job->next = jobs_remove_job(job->next, pid);
	return (job);
}

int				jobs_add_job(t_vshdata *vshdata, pid_t pid, char *command)
{
	t_job	*job;
	int		jid;

	if (vshdata->jobs->joblist == NULL)
	{
		vshdata->jobs->joblist = new_job(pid, 1, command,
			builtin_jobs_new_current_val(vshdata->jobs->joblist));
		if (vshdata->jobs->joblist == NULL)
			return (FUNCT_ERROR);
	}
	else
	{
		job = vshdata->jobs->joblist;
		jid = job->job_id;
		while (job->next)
		{
			job = job->next;
			if (job->job_id > jid)
				jid = job->job_id;
		}
		job->next = new_job(pid, jid + 1, command,
			builtin_jobs_new_current_val(vshdata->jobs->joblist));
		if (job->next == NULL)
			return (FUNCT_ERROR);
	}
	return (FUNCT_SUCCESS);
}
