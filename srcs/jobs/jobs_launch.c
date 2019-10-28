/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   jobs_launch.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: rkuijper <rkuijper@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/10/28 16:25:10 by rkuijper       #+#    #+#                */
/*   Updated: 2019/10/28 23:06:08 by jbrinksm      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"

static void	jobs_launch_proc(t_job *job, t_proc *proc, int fds[3], int pipes[2])
{
	pid_t pid;

	if (g_state->shell_type == SHELL_INTERACT)
	{
		pid = getpid();
		if (job->pgid == 0)
			job->pgid = pid;
		setpgid(0, job->pgid);
		if (job->bg == true)
			tcsetpgrp(STDIN_FILENO, job->pgid);
		signal_reset();
	}
	if (fds[0] != STDIN_FILENO)
	{
		dup2(fds[0], STDIN_FILENO);
		close(fds[0]);
	}
	else
		close(pipes[0]); /* @rob dit was de fix: When the childs STDIN is the terminal, the pipe is not used so therefore it should be closed in the child process */
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
	if (exec_redirs(proc->redir_node) == FUNCT_ERROR) /* Do redirections based on the redir node linked to the process */
		return ;

	execve(proc->binary, proc->argv, proc->env);
	ft_eprintf("Error executing %s\n", proc->binary);
	exit(1);
}

void		jobs_launch_job(t_job *job)
{
	pid_t	pid;
	t_proc	*proc;
	int		fds[3];
	int		pipes[2];

	fds[0] = STDIN_FILENO;
	fds[2] = STDERR_FILENO;
	proc = job->processes;
	signal(SIGCHLD, SIG_DFL);
	while (proc != NULL)
	{
		if (proc->next != NULL)
		{
			if (pipe(pipes) < 0)
			{
				ft_eprintf("Could not pipe.\n");
				exit(1);
			}
			fds[1] = pipes[1];
		}
		else
			fds[1] = STDOUT_FILENO;

		pid = fork();
		if (pid < 0) 	/* Fork failed */
		{
			ft_eprintf("Could not fork.\n");
			exit(1);
		}
		if (pid == 0) 	/* Child */
			jobs_launch_proc(job, proc, fds, pipes);
		else 			/* Parent */
		{
			proc->pid = pid;
			if (g_state->shell_type == SHELL_INTERACT)
			{
				if (job->pgid == 0)
					job->pgid = pid;
				setpgid(pid, job->pgid);
			}
		}

		if (fds[0] != STDIN_FILENO)
			close(fds[0]);
		if (fds[1] != STDOUT_FILENO)
			close(fds[1]);
		fds[0] = pipes[0];

		proc = proc->next;
	}

	if (job->bg == true)
		jobs_bg_job(job, false);
	else
		jobs_fg_job(job, false);
	signal(SIGCHLD, signal_handle_child_death);
}
