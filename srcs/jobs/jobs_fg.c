/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   jobs_fg.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: rkuijper <rkuijper@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/10/18 17:12:11 by rkuijper       #+#    #+#                */
/*   Updated: 2019/10/31 16:32:12 by rkuijper      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"
#include <term.h>
#include <signal.h>
#include <stdio.h>

static int		job_stop(t_job *job)
{
	if (g_state->shell_type == SHELL_INTERACT)
		ft_printf("\r\x1b[0K^Z\n");
	job->bg = true;
	jobs_print_job_info(job, JOB_OPT_L, g_data->jobs->joblist);
	return (146);
}

static int		fg_wait_and_reset(t_job *job, bool job_continued)
{
	jobs_force_job_state(job, job_continued ? PROC_CONTINUED : PROC_RUNNING);
	g_state->exit_code = jobs_wait_job(job, WUNTRACED);
	if (g_state->shell_type == SHELL_INTERACT)
	{
		tcsetpgrp(STDIN_FILENO, g_state->pid);
		tcsetattr(STDIN_FILENO, TCSADRAIN, g_data->term->termios_p);
	}
	if (jobs_stopped_job(job))
		job_stop(job);
	else
		jobs_finished_job(job, true);
	return (g_state->exit_code);
}

int				jobs_fg_job(t_job *job, bool job_continued)
{
	job->bg = false;
	if (g_state->shell_type == SHELL_INTERACT)
		tcsetpgrp(STDIN_FILENO, job->pgid);
	tcsetattr(STDIN_FILENO, TCSADRAIN, g_data->term->old_termios_p);
	if (job_continued == true)
	{
		if (kill(-job->pgid, SIGCONT) < 0)
			return (ft_eprintf("Could not SIGCONT job %i\n", job->pgid));
	}
	return (fg_wait_and_reset(job, job_continued));
}
