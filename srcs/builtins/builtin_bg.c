/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   builtin_bg.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: rkuijper <rkuijper@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/10/18 15:54:56 by rkuijper       #+#    #+#                */
/*   Updated: 2019/10/30 14:54:38 by rkuijper      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"

static int	read_options(char **av)
{
	if (av[1] == NULL || av[1][0] != '-')
		return (FUNCT_SUCCESS);
	else
	{
		ft_eprintf(E_BG_INV_OPT E_BG_USAGE, av[1][1]);
		return (FUNCT_ERROR);
	}
}

/*
	TODO:
	Calling bg on a suspended process does not continue the process correctly.
*/

void		builtin_bg(char **av, t_vshdata *data)
{
	t_job	*job;

	g_state->exit_code = EXIT_FAILURE;
	if (read_options(av) == FUNCT_ERROR)
		return ;
	if (av[1] == NULL)
		job = jobs_find_job("%%", data->jobs->joblist);
	else
		job = jobs_find_job(av[1], data->jobs->joblist);
	if (job == NULL)
	{
		ft_eprintf(E_BG_NO_CUR);
		return ;
	}
	if (tools_get_pid_state(-job->pgid) == PID_STATE_SUSPEND)
		jobs_continue_job(job, false);
	else
	{
		ft_eprintf(E_BG_JOB_RUN, av[1] == NULL ? "1" : av[1]);
		return ;
	}
	g_state->exit_code = EXIT_SUCCESS;
}