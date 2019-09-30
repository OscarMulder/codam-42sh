/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   builtin_jobs.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: omulder <omulder@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/04/17 14:03:16 by rkuijper       #+#    #+#                */
/*   Updated: 2019/09/30 16:12:22 by tde-jong      ########   odam.nl         */
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

static t_job	*find_current_job(t_job *joblist)
{
	t_job *job;
	t_job *toreturn;

	job = joblist;
	toreturn = job;
	while (job != NULL)
	{
		if (job->current > toreturn->current)
			toreturn = job;
		job = job->next;
	}
	return (toreturn);
}

int				builtin_jobs_new_current_val(t_job *joblist)
{
	t_job *job;

	job = find_current_job(joblist);
	if (job == NULL)
		return (0);
	else
		return (job->current + 1);
}

static t_job	*find_previous_job(t_job *joblist)
{
	t_job *job;
	t_job *toreturn;
	t_job *current_job;

	current_job = find_current_job(joblist);
	if (find_current_job(joblist) == joblist
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

static bool		is_current_job(t_job *job, t_job *joblist)
{
	if (find_current_job(joblist) == job)
		return (true);
	return (false);
}

static bool		is_previous_job(t_job *job, t_job *joblist)
{
	if (find_previous_job(joblist) == job && job->next != NULL)
		return (true);
	return (false);
}

static t_job	*find_job_contains_str(char *str, t_job *joblist)
{
	t_job *job;

	job = joblist;
	while (job != NULL)
	{
		if (ft_strstr(job->command_name, str) != NULL)
			return (job);
		job = job->next;
	}
	return (NULL);
}

static t_job	*find_job_startswith_str(char *str, t_job *joblist)
{
	t_job *job;

	job = joblist;
	while (job != NULL)
	{
		if (ft_strstr(job->command_name, str) == job->command_name)
			return (job);
		job = job->next;
	}
	return (NULL);
}

static t_job	*find_job_n(char *n, t_job *joblist)
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

t_job			*builtin_jobs_find_job(char *job_id, t_job *joblist)
{
	if (job_id != NULL && job_id[0] != '\0' && job_id[1] != '\0')
	{
		job_id++;
		/* In this case we return Current Job */
		if ((*job_id == '%' || *job_id == '+') && *(job_id + 1) == '\0')
			return (find_current_job(joblist));
		/* In this case we return Previous job. */
		else if (*job_id == '-' && *(job_id + 1) == '\0')
			return (find_previous_job(joblist));
		/* In this case we return Job whose command contains string. */
		else if (*job_id == '?')
			return (find_job_contains_str(job_id + 1, joblist));
		/* In this case we return Job number n. */
		else if (ft_isdigit(*job_id))
			return (find_job_n(job_id, joblist));
		/* In this case we return Job whose command begins with string. */
		else
			return (find_job_startswith_str(job_id, joblist));
	}
	return (NULL);
}

static void	print_job_info(t_job *job, int options, t_job *joblist)
{
	int	current;

	current = ' ';
	if (is_current_job(job, joblist))
		current = '+';
	if (is_previous_job(job, joblist))
		current = '-';
	ft_printf("[%i] %c ", job->job_id, current);
	if (options & JOB_OPT_L || options & JOB_OPT_P)
		ft_printf("%i ", job->process_id);
	ft_printf("%s ", jobs_get_job_state(job) == JOB_RUNNING ?
		"running" : "suspended");
	ft_printf("%s\n", job->command_name);
}

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
	t_job	*toprint;

	(void)options;
	if (jobs->joblist == NULL)
	{
		ft_eprintf("jobs: %s: no such job", args[0]);
		return (FUNCT_ERROR);
	}
	i = 0;
	while (args[i] != NULL)
	{
		toprint = builtin_jobs_find_job(args[i], jobs->joblist);
		if (toprint == NULL)
			return (FUNCT_FAILURE);
		print_job_info(toprint, options, jobs->joblist);
		i++;
	}
	return (FUNCT_SUCCESS);
}

static int	jobs_log_all(t_datajobs *jobs, int options)
{
	t_job *job;

	if (jobs->joblist == NULL)
		return (FUNCT_SUCCESS);
	job = jobs->joblist;
	while (job != NULL)
	{
		if (job->process_id == 0)
		{
			job = job->next;
			continue ;
		}
		print_job_info(job, options, jobs->joblist);
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
		return (jobs_log_all(data->jobs, options));
	return (FUNCT_SUCCESS);
}
