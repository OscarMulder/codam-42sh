/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   builtin_bg.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: rkuijper <rkuijper@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/10/18 15:54:56 by rkuijper       #+#    #+#                */
/*   Updated: 2019/10/18 17:13:40 by rkuijper      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"

static int	read_options(char **av)
{
	while (*av != NULL)
	{
		if (*av[0] != '-')
			break ;
		else
		{
			ft_eprintf("vsh: bg: %s: invalid option\n");
			ft_eprintf("bg: usage: bg [job_spec ...]\n");
			return (FUNCT_ERROR);
		}
		av++;
	}
	return (FUNCT_SUCCESS);
}

static int	bg_with_params(char **av, t_vshdata *data)
{
	int num;

	if (read_options(av) == FUNCT_ERROR)
		return (FUNCT_ERROR);
	while (*av != NULL)
	{
		num = ft_atoi(*av);
		if (ft_strlen(*av) != ft_numlen(num))
			ft_eprintf("bg: %s: job not found\n");
		else
			
		av++;
	}
	(void)data;
}

int			builtin_bg(char **av, t_vshdata *data)
{
	t_job	*job;

	job = data->jobs->joblist;
	av++;
	if (*av != NULL)
		return (bg_with_params(av, data));
	while (job != NULL)
	{
		if (tools_get_pid_state(job->pgid) == PID_STATE_SUSPEND)
		{
			jobs_continue_job(job, 0);
			return (FUNCT_SUCCESS);
		}
		job = job->next;
	}
	return (err_ret("bg: no current job\n"));
}
