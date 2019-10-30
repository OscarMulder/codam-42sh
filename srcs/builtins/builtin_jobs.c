/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   builtin_jobs.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: omulder <omulder@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/04/17 14:03:16 by rkuijper       #+#    #+#                */
/*   Updated: 2019/10/30 16:37:40 by rkuijper      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"

int				builtin_jobs_new_current_val(t_job *joblist)
{
	t_job *job;

	job = jobs_find_current_job(joblist);
	if (job == NULL)
		return (0);
	else
		return (job->current + 1);
}

static int		read_options(char **args, int *arg, int *options)
{
	int i;

	while (args[*arg] != NULL && *args[*arg] == '-')
	{
		i = 1;
		while (args[*arg][i] != '\0')
		{
			if (args[*arg][i] == 'p')
				*options |= JOB_OPT_P;
			else if (args[*arg][i] == 'l')
				*options |= JOB_OPT_L;
			else
			{
				ft_eprintf(E_JOBS_USAGE, args[*arg][i]);
				return (FUNCT_ERROR);
			}
			i++;
		}
		(*arg)++;
	}
	return (FUNCT_SUCCESS);
}

static int		jobs_log_args(t_datajobs *jobs, int options, char **args)
{
	int		i;
	t_job	*toprint;

	if (jobs->joblist == NULL)
	{
		ft_eprintf(E_JOBS_NO_JOB, args[0]);
		return (FUNCT_ERROR);
	}
	i = 0;
	while (args[i] != NULL)
	{
		toprint = jobs_find_job(args[i], jobs->joblist);
		if (toprint == NULL)
			return (FUNCT_FAILURE);
		jobs_print_job_info(toprint, options, jobs->joblist);
		i++;
	}
	return (FUNCT_SUCCESS);
}

static int		jobs_log_all(t_datajobs *jobs, int options)
{
	t_job *job;

	job = jobs->joblist;
	while (job != NULL)
	{
		if (job->pgid == 0)
		{
			job = job->next;
			continue ;
		}
		jobs_print_job_info(job, options, jobs->joblist);
		job = job->next;
	}
	return (FUNCT_SUCCESS);
}

int				builtin_jobs(char **args, t_vshdata *data)
{
	int	arg;
	int options;

	arg = 1;
	options = JOB_OPT_NONE;
	if (read_options(args, &arg, &options) != FUNCT_SUCCESS)
		return (FUNCT_ERROR);
	jobs_handle_finished_jobs();
	if (args[arg] != NULL)
		jobs_log_args(data->jobs, options, args + arg);
	else
		return (jobs_log_all(data->jobs, options));
	return (FUNCT_SUCCESS);
}
