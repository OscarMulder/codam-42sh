/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   builtin_fg.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: rkuijper <rkuijper@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/10/22 14:27:21 by rkuijper       #+#    #+#                */
/*   Updated: 2019/10/22 14:34:19 by rkuijper      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"

static int	read_options(char **av)
{
	if (av[1] == NULL || av[1][0] != '-')
		return (FUNCT_SUCCESS);
	else
	{
		ft_eprintf("vsh: bg: %s: invalid option\n");
		ft_eprintf("bg: usage: bg [job_spec ...]\n");
		return (FUNCT_ERROR);
	}
}

static void	fg(t_job *job)
{
	t_proc *proc;

	if (jobs_stopped_job(job))
		jobs_continue_job(job, true);
	else
	{
		proc = job->processes;
		while (proc != NULL)
		{
			proc->state = PROC_RUNNING;
			proc = proc->next;
		}
		jobs_print_job_info(job, JOB_OPT_L, g_data->jobs->joblist);
		jobs_fg_job(job, 0);
	}
}

int			builtin_fg(char **av, t_vshdata *data)
{
	t_job	*job;

	if (read_options(av) == FUNCT_ERROR)
		return (FUNCT_ERROR);
	if (av[1] == NULL)
		job = jobs_find_job("%%", data->jobs->joblist);
	else
		job = jobs_find_job(av[1], data->jobs->joblist);
	if (job == NULL)
		return (err_ret("fg: no current job\n"));
	job->current = builtin_jobs_new_current_val(data->jobs->joblist);
	fg(job);
	return (FUNCT_SUCCESS);
}
