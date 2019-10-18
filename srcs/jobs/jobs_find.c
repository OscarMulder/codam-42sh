/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   jobs_find.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: rkuijper <rkuijper@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/10/18 17:08:34 by rkuijper       #+#    #+#                */
/*   Updated: 2019/10/18 17:11:14 by rkuijper      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"

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
