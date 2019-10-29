/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   jobs_launch_proc.c                                 :+:    :+:            */
/*                                                     +:+                    */
/*   By: rkuijper <rkuijper@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/10/29 11:20:31 by rkuijper       #+#    #+#                */
/*   Updated: 2019/10/29 13:51:18 by jbrinksm      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"

static void	handle_filedescriptors(int fds[3], int pipes[2])
{
	if (fds[0] != STDIN_FILENO)
	{
		dup2(fds[0], STDIN_FILENO);
		close(fds[0]);
	}
	else
		close(pipes[0]);
	if (fds[1] != STDOUT_FILENO)
	{
		dup2(fds[1], STDOUT_FILENO);
		close(fds[1]);
	}
	if (fds[2] != STDERR_FILENO)
	{
		dup2(fds[2], STDERR_FILENO);
		close(fds[2]);
	}
}

void		jobs_launch_proc(t_job *job, t_proc *proc, int fds[3], int pipes[2])
{
	pid_t pid;

	if (g_state->shell_type == SHELL_INTERACT)
	{
		pid = getpid();
		if (job->pgid == 0)
			job->pgid = pid;
		setpgid(0, job->pgid);
		if (job->bg == false)
			tcsetpgrp(STDIN_FILENO, job->pgid);
		signal_reset();
	}
	handle_filedescriptors(fds, pipes);
	if (exec_redirs(proc->redir_node) == FUNCT_ERROR)
		return ;
	execve(proc->binary, proc->argv, proc->env);
	ft_eprintf("Error executing %s\n", proc->binary);
	exit(1);
}
