/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   builtin_jobs.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: omulder <omulder@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/04/17 14:03:16 by rkuijper       #+#    #+#                */
/*   Updated: 2019/09/10 19:51:07 by anonymous     ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"

/*
**	Parse options -p and -l
**	There's also the 'job' parameter which can be present in args, but it doesn't work(?)
**
*/

#define JOB_OPT_NONE	0
#define JOB_OPT_P		1
#define JOB_OPT_L		2

static int	read_options(char **args, int *arg, int *options)
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
				ft_eprintf("vsh: bad option: -%c\n", args[*arg][i]);
				return (FUNCT_ERROR);
			}
			i++;
		}
		(*arg)++;
	}
	return (FUNCT_SUCCESS);
}

/* This needs to loop over every job and print it out if it's in the args list. */
static int	jobs_log_args(t_datajobs *jobs, int options, char **args)
{
	int		i;
	t_job	*job;
	/* 	If joblist is null right off the bat, we know there are no jobs we can loop through,
		so return with an error code immediately. */
	(void)options;
	if (jobs->joblist == NULL)
	{
		ft_eprintf("jobs: %s: no such job", args[0]);
		return (FUNCT_ERROR);
	}
	i = 0;
	while (args[i] != NULL)
	{
		job = jobs->joblist;
		while (job != NULL)
		{
			/* I'm not too sure how this has to be parsed. Something for the near future. */
			job = job->next;
		}
		i++;
	}
	return (FUNCT_SUCCESS);
}

static int	jobs_log_all(t_datajobs *jobs, int options)
{
	t_job *job;
	/* If job list is empty, just return a success state. */
	if (jobs->joblist == NULL)
		return (FUNCT_SUCCESS);
	/* Start looping through all jobs. */
	job = jobs->joblist;
	while (job != NULL)
	{
		if (job->process_id == 0)
		{
			job = job->next;
			continue;
		}
		if (options & JOB_OPT_L || options & JOB_OPT_P)
			ft_printf("[%i]  + %i %s  %s\n", job->job_id, job->process_id,
				job->state == 0 ? "running" : "suspended", job->command_name);
		else
			ft_printf("[%i]  + %s %s\n", job->job_id, job->state == 0 ? "running" : "suspended", job->command_name);
		job = job->next;
	}
	return (FUNCT_SUCCESS);
}

int			builtin_jobs(char **args, t_vshdata *data)
{
	int	arg;
	int options;

	arg = 1;
	options = JOB_OPT_NONE;
	/* Try to read out all options. */
	if (read_options(args, &arg, &options) != FUNCT_SUCCESS)
		return (FUNCT_ERROR);
	/* If the job list is empty, just return and do nothing. */
	if (args[arg] != NULL) /* Start parsing the rest of the parameters, only listing the given jobs. */
		jobs_log_args(data->jobs, options, args + arg);
	else /* No parameters left after options, start logging ALL jobs. */
		jobs_log_all(data->jobs, options);
	return (FUNCT_SUCCESS);
}
