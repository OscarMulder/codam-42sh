/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   builtin_jobs_find_job.c                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: tde-jong <tde-jong@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/09/17 16:37:33 by tde-jong       #+#    #+#                */
/*   Updated: 2019/09/18 16:45:39 by tde-jong      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"

static t_job	*find_current_job(t_job *joblist)
{
	t_job *job;

	job = joblist;
	while (job != NULL)
	{
		if (job->current == 1)
			return (job);
		job = job->next;
	}
	return (NULL);
}

t_job			*builtin_jobs_find_job(char *job_id, t_job *joblist)
{
	if (job_id != NULL && job_id[0] != '\0' && job_id[1] != '\0')
	{
		job_id++;
		/* In this case we return Current Job */
		if (*job_id == '%' || *job_id == '+')
			return (find_current_job(joblist));
		/* In this case we return Previous job. */
		else if (*job_id == '-')
		{

		}
		/* In this case we return Job whose command contains string. */
		else if (*job_id == '?')
		{

		}
		/* In this case we return Job number n. */
		else if (ft_isdigit(*job_id))
		{

		}
		/* In this case we return Job whose command begins with string. */
		else
		{

		}
	}
	return (NULL);
}
