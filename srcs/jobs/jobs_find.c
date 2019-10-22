/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   jobs_find.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: rkuijper <rkuijper@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/10/18 17:08:34 by rkuijper       #+#    #+#                */
/*   Updated: 2019/10/21 12:20:52 by rkuijper      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"

t_job			*jobs_find_job(char *job_id, t_job *joblist)
{
	if (job_id != NULL && job_id[0] != '\0' && job_id[1] != '\0')
	{
		job_id++;
		/* In this case we return Current Job */
		if ((*job_id == '%' || *job_id == '+') && *(job_id + 1) == '\0')
			return (jobs_find_current_job(joblist));
		/* In this case we return Previous job. */
		else if (*job_id == '-' && *(job_id + 1) == '\0')
			return (jobs_find_previous_job(joblist));
		/* In this case we return Job whose command contains string. */
		else if (*job_id == '?')
			return (jobs_find_contains_str(job_id + 1, joblist));
		/* In this case we return Job number n. */
		else if (ft_isdigit(*job_id))
			return (jobs_find_n(job_id, joblist));
		/* In this case we return Job whose command begins with string. */
		else
			return (jobs_find_startswith_str(job_id, joblist));
	}
	return (NULL);
}

t_job			*jobs_find_previous_job(t_job *joblist)
{
	t_job *job;
	t_job *toreturn;
	t_job *current_job;

	current_job = jobs_find_current_job(joblist);
	if (current_job == joblist
		|| (joblist != NULL && joblist->next == NULL))
		return (NULL);
	job = joblist;
	toreturn = job;
	while (job != NULL && job->next != NULL)
	{
		if (job->current < current_job->current
			&& job->current > toreturn->current)
			toreturn = job;
		job = job->next;
	}
	return (toreturn);
}

t_job	*jobs_find_contains_str(char *str, t_job *joblist)
{
	t_job *job;

	job = joblist;
	while (job != NULL)
	{
		if (ft_strstr(job->command, str) != NULL)
			return (job);
		job = job->next;
	}
	return (NULL);
}

t_job	*jobs_find_startswith_str(char *str, t_job *joblist)
{
	t_job *job;

	job = joblist;
	while (job != NULL)
	{
		if (ft_strstr(job->command, str) == job->command)
			return (job);
		job = job->next;
	}
	return (NULL);
}

t_job	*jobs_find_n(char *n, t_job *joblist)
{
	t_job *job;

	job = joblist;
	while (job != NULL)
	{
		if (ft_atoi(n) == job->job_id)
			return (job);
		job = job->next;
	}
	return (NULL);
}
