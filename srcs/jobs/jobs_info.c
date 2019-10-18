/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   jobs_info.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: rkuijper <rkuijper@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/10/18 16:48:34 by rkuijper       #+#    #+#                */
/*   Updated: 2019/10/18 17:01:20 by rkuijper      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"

void	jobs_print_job_info(t_job *job, int options, t_job *joblist)
{
	int	current;

	current = ' ';
	if (is_current_job(job, joblist))
		current = '+';
	if (is_previous_job(job, joblist))
		current = '-';
	ft_printf("[%i]%c ", job->job_id, current);
	if (options & (JOB_OPT_L | JOB_OPT_P))
		ft_printf("%i ", job->pgid);
	ft_printf("%s ", jobs_get_job_state(job) == JOB_RUNNING ?
		"Running" : "Stopped");
	ft_printf("%s\n", job->command);
}
