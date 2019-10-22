/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   builtin_bg.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: rkuijper <rkuijper@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/10/18 15:54:56 by rkuijper       #+#    #+#                */
/*   Updated: 2019/10/21 12:18:14 by rkuijper      ########   odam.nl         */
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

int			builtin_bg(char **av, t_vshdata *data)
{
	t_job	*job;

	if (read_options(av) == FUNCT_ERROR)
		return (FUNCT_ERROR);
	if (av[1] == NULL)
		job = jobs_find_job("%%", data->jobs->joblist);
	else
		job = jobs_find_job(av[1], data->jobs->joblist);
	if (job == NULL)
		return (err_ret("bg: no current job\n"));
	if (tools_get_pid_state(job->pgid) == PID_STATE_SUSPEND)
		jobs_continue_job(job, 0);
	else
	{
		ft_eprintf("vsh: bg: job %s already running\n",
			av[1] == NULL ? "1" : av[1]);
		return (FUNCT_ERROR);
	}
	return (FUNCT_SUCCESS);
}

