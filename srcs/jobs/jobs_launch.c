/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   jobs_launch.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: rkuijper <rkuijper@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/10/28 16:25:10 by rkuijper       #+#    #+#                */
/*   Updated: 2019/10/31 09:46:27 by rkuijper      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"
#include "signal.h"
#include <unistd.h>

// static int	jobs_launch_pipe(t_job *job, t_proc *proc, int *fds, int *pipes)
// {
// 	if (proc->is_builtin == false && proc->binary == NULL)
// 	{
// 		jobs_flush_job(job);
// 		g_state->exit_code = 1;
// 		return (FUNCT_ERROR);
// 	}
// 	if (proc->next != NULL)
// 	{
// 		if (pipe(pipes) < 0)
// 		{
// 			ft_eprintf("Could not pipe.\n");
// 			exit(1);
// 		}
// 		fds[1] = pipes[1];
// 	}
// 	else
// 		fds[1] = STDOUT_FILENO;
// 	return (FUNCT_SUCCESS);
// }

// static int	init_fds_and_pipe(int **pipes, int **fds)
// {
// 	*pipes = ft_memalloc(sizeof(pipes) * 2);
// 	*fds = ft_memalloc(sizeof(int) * 3);
// 	if (*pipes == NULL || *fds == NULL)
// 	{
// 		ft_eprintf(E_N_ALLOC_STR, "fork");
// 		return (FUNCT_ERROR);
// 	}
// 	(*fds)[0] = STDIN_FILENO;
// 	(*fds)[2] = STDERR_FILENO;
// 	(*pipes)[0] = UNINIT;
// 	(*pipes)[1] = UNINIT;
// 	return (FUNCT_SUCCESS);
// }

// static void	jobs_launch_fork(t_job *job, t_proc *proc, int *fds, int *pipes)
// {
// 	pid_t	pid;

// 	pid = fork();
// 	if (pid < 0)
// 	{
// 		ft_eprintf("Could not fork.\n");
// 		exit(1);
// 	}
// 	if (pid == 0)
// 		jobs_launch_proc(job, proc, fds, pipes);
// 	else
// 	{
// 		proc->pid = pid;
// 		if (g_state->shell_type == SHELL_INTERACT)
// 		{
// 			if (job->pgid == 0)
// 				job->pgid = pid;
// 			setpgid(pid, job->pgid);
// 		}
// 	}
// }

// static void	jobs_launch_cleanup(t_proc *proc, int *fds, int *pipes)
// {
// 	if (fds[0] != STDIN_FILENO && fds[0] != UNINIT)
// 		close(fds[0]);
// 	if (fds[1] != STDOUT_FILENO && fds[1] != UNINIT)
// 		close(fds[1]);
// 	if (proc->binary != NULL)
// 		ft_strdel(&proc->binary);
// 	if (proc->argv != NULL)
// 		ft_strarrdel(&proc->argv);
// 	if (proc->env)
// 		ft_strarrdel(&proc->env);
// 	fds[0] = pipes[0];
// }


/* Was al bezig om deze norm proof te maken, zorg dat je goed kijkt dat de fds
en pipes worden geinitialiseerd/aangepast */

void		jobs_launch_job(t_job *job)
{
	pid_t	pid;
	t_proc	*proc;
	int		fds[3];
	int		pipes[2];

	fds[0] = STDIN_FILENO;
	fds[2] = STDERR_FILENO;
	ft_memset(pipes, UNINIT, sizeof(pipes));
	proc = job->processes;
	if (job->bg == false && jobs_exec_is_single_builtin_proc(proc))
	{
		jobs_exec_builtin(job->processes);
		jobs_finished_job(job, true);
		return ;
	}
	else
	{
		while (proc != NULL)
		{
			if (proc->is_builtin == false && proc->binary == NULL)
			{
				jobs_flush_job(job);
				g_state->exit_code = 1;
				return ;
			}
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
			if (pid < 0)
			{
				ft_eprintf("Could not fork.\n");
				exit(1);
			}
			if (pid == 0)
				jobs_launch_proc(job, proc, fds, pipes);
			else
			{
				proc->pid = pid;
				if (g_state->shell_type == SHELL_INTERACT)
				{
					if (job->pgid == 0)
						job->pgid = pid;
					setpgid(pid, job->pgid);
				}
			}
			if (fds[0] != STDIN_FILENO && fds[0] != UNINIT)
				close(fds[0]);
			if (fds[1] != STDOUT_FILENO && fds[1] != UNINIT)
				close(fds[1]);
			fds[0] = pipes[0];
			if (proc->binary != NULL)
				ft_strdel(&proc->binary);
			if (proc->argv != NULL)
				ft_strarrdel(&proc->argv);
			if (proc->env)
				ft_memdel((void**)&proc->env);
			proc = proc->next;
		}
	}
	if (job->bg == true)
		jobs_bg_job(job, false);
	else
		g_state->exit_code = jobs_fg_job(job, false);
}
