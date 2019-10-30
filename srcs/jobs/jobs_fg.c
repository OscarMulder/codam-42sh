/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   jobs_fg.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: rkuijper <rkuijper@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/10/18 17:12:11 by rkuijper       #+#    #+#                */
/*   Updated: 2019/10/30 18:11:30 by rkuijper      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"
#include <term.h>
#include <signal.h>
#include <stdio.h>

static t_proc	*last_proc(t_job *job)
{
	t_proc *proc;

	if (job == NULL || job->processes == NULL)
		return (NULL);
	proc = job->processes;
	while (proc->next != NULL)
		proc = proc->next;
	return (proc);
}

static void 	print_job_tree(void)
{
	t_job *job;

	job = g_data->jobs->joblist;
	while (job != NULL)
	{
		ft_eprintf("Job nr %i: %s\n", job->pgid, job->command);
		job = job->next;
	}
}

static int		job_stop(t_job *job)
{
	jobs_add_job(g_data, job);
	if (g_state->shell_type == SHELL_INTERACT)
		ft_printf("\r\x1b[0K^Z\n");
	job->bg = true;
	ft_eprintf("\e[33mJob tree after stop:\n");
	print_job_tree();
	ft_eprintf("\n\e[0m");
	jobs_print_job_info(job, JOB_OPT_L, g_data->jobs->joblist);
	return (146);
}

static void		job_finished(t_job *job, bool check)
{
	t_job *child;

	if (job == NULL)
		return ;
	jobs_remove_job(&g_data->jobs->joblist, job->pgid);
	child = job->child;
	if (check && job->child != NULL &&
		(job->andor == ANDOR_NONE ||
		(job->andor == ANDOR_AND && g_state->exit_code == 0) ||
		(job->andor == ANDOR_OR && g_state->exit_code != 0)))
	{
		jobs_flush_job(job);
		jobs_launch_job(child);
		return ;
	}
	else if (child != NULL)
		job_finished(child, false);
	jobs_flush_job(job);
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
	jobs_wait_job(job);
	if (g_state->shell_type == SHELL_INTERACT)
	{
		tcsetpgrp(STDIN_FILENO, g_state->pid);
		tcsetattr(STDIN_FILENO, TCSADRAIN, g_data->term->termios_p);
	}
	if (jobs_stopped_job(job))
		g_state->exit_code = job_stop(job);
	else
	{
		if (last_proc(job) != NULL)
			g_state->exit_code = last_proc(job)->exit_status;
		job_finished(job, true);
		ft_eprintf("\e[33mJob tree after INTERUPT BIATCH:\n");
		print_job_tree();
		ft_eprintf("\n\e[0m");
	}
	return (g_state->exit_code);
}
