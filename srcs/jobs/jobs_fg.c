/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   jobs_fg.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: rkuijper <rkuijper@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/10/18 17:12:11 by rkuijper       #+#    #+#                */
/*   Updated: 2019/10/22 14:16:21 by rkuijper      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"
#include <term.h>
#include <errno.h>
#include <signal.h>

static t_proc	*last_proc(t_job *job)
{
	t_proc *proc;

	proc = job->processes;
	while (proc->next != NULL)
		proc = proc->next;
	return (proc);
}

static int		job_stop(t_job *job)
{
	if (g_state->shell_type == SHELL_INTERACT)
		ft_printf("\r\x1b[0K^Z\n");
	job->bg = true;
	jobs_print_job_info(job, JOB_OPT_L, g_data->jobs->joblist);
	return (146);
}

int				jobs_fg_job(t_job *job, bool job_continued)
{
	int status;

	status = 0;
	job->bg = false;
	if (g_state->shell_type == SHELL_INTERACT)
		tcsetpgrp(STDIN_FILENO, job->pgid);
	if (job_continued)
	{
		tcsetattr(STDIN_FILENO, TCSADRAIN, job->tmode);
		if (kill(-job->pgid, SIGCONT) < 0)
			return (ft_eprintf("kill (SIGCONT): %i\n", errno));
	}
	jobs_wait_job(job);
	if (g_state->shell_type == SHELL_INTERACT)
	{
		tcsetpgrp(STDIN_FILENO, g_state->pid);
		tcgetattr(STDIN_FILENO, job->tmode);
		tcsetattr(STDIN_FILENO, TCSADRAIN, g_data->term->termios_p);
	}
	if (jobs_stopped_job(job))
		status = job_stop(job);
	else
		status = last_proc(job)->exit_status;
	return (status);
}
