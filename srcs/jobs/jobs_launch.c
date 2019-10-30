/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   jobs_launch.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: rkuijper <rkuijper@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/10/28 16:25:10 by rkuijper       #+#    #+#                */
/*   Updated: 2019/10/30 16:21:41 by jbrinksm      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"

static int	is_single_builtin_proc(t_proc *proc)
{
	return (proc != NULL && proc->is_builtin == true && proc->next == NULL);
}

static int	cleanup_non_forked_redirs(void)
{
	if (dup2(g_data->stdfds[0], STDIN_FILENO) == -1)
		return (FUNCT_ERROR);
	close(g_data->stdfds[0]);
	if (dup2(g_data->stdfds[1], STDOUT_FILENO) == -1)
		return (FUNCT_ERROR);
	close(g_data->stdfds[1]);
	if (dup2(g_data->stdfds[2], STDERR_FILENO) == -1)
		return (FUNCT_ERROR);
	close(g_data->stdfds[2]);
	return (FUNCT_SUCCESS);
}

static int	backup_stdfds(void)
{
	g_data->stdfds[0] = dup(STDIN_FILENO);
	if (g_data->stdfds[0] == -1)
		return (FUNCT_ERROR);
	g_data->stdfds[1] = dup(STDOUT_FILENO);
	if (g_data->stdfds[1] == -1)
		return (FUNCT_ERROR);
	g_data->stdfds[2] = dup(STDERR_FILENO);
	if (g_data->stdfds[2] == -1)
		return (FUNCT_ERROR);
	return (FUNCT_SUCCESS);
}

static void	execute_builtin(t_proc *proc)
{
	if (backup_stdfds() == FUNCT_ERROR)
	{
		g_state->exit_code = EXIT_FAILURE;
		ft_eprintf(E_FAIL_DUP_FD);
	}
	if (redir(proc->redir_and_assign) == FUNCT_ERROR)
	{
		g_state->exit_code = EXIT_FAILURE;
		return ;
	}
	if (ft_strequ(proc->argv[0], "echo"))
		builtin_echo(proc->argv);
	if (ft_strequ(proc->argv[0], "exit"))
		builtin_exit(proc->argv, g_data);
	if (ft_strequ(proc->argv[0], "cd"))
		builtin_cd(proc->argv, g_data);
	if (ft_strequ(proc->argv[0], "fc"))
		builtin_fc(proc->argv, g_data);
	if (ft_strequ(proc->argv[0], "export"))
		builtin_export(proc->argv, g_data);
	if (ft_strequ(proc->argv[0], "set"))
		builtin_set(proc->argv, g_data->envlst);
	if (ft_strequ(proc->argv[0], "unset"))
		builtin_unset(proc->argv, g_data->envlst);
	if (ft_strequ(proc->argv[0], "history"))
		history_print(g_data->history);
	if (ft_strequ(proc->argv[0], "type"))
		builtin_type(proc->argv, g_data->envlst, g_data->alias->aliaslst);
	if (ft_strequ(proc->argv[0], "alias"))
		builtin_alias(proc->argv, &g_data->alias->aliaslst);
	if (ft_strequ(proc->argv[0], "unalias"))
		builtin_unalias(proc->argv, &g_data->alias->aliaslst);
	if (ft_strequ(proc->argv[0], "jobs"))
		builtin_jobs(proc->argv, g_data);
	if (ft_strequ(proc->argv[0], "fg"))
		builtin_fg(proc->argv, g_data);
	if (ft_strequ(proc->argv[0], "bg"))
		builtin_bg(proc->argv, g_data);
	if (ft_strequ(proc->argv[0], "hash"))
		builtin_hash(proc->argv, g_data);
	if (cleanup_non_forked_redirs() == FUNCT_ERROR)
	{
		g_state->exit_code = EXIT_FAILURE;
		ft_eprintf(E_FD_RESET_STD);
	}
}

void		jobs_launch_job(t_job *job)
{
	pid_t	pid;
	t_proc	*proc;
	int		fds[3];
	int		pipes[2];

	fds[0] = STDIN_FILENO;
	fds[2] = STDERR_FILENO;
	pipes[0] = UNINIT;
	pipes[1] = UNINIT;
	proc = job->processes;
	if (is_single_builtin_proc(proc))
		execute_builtin(job->processes);
	else
	{
		signal(SIGCHLD, SIG_DFL);
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
			// TODO: Reimplement this when the environment is working!
			// if (proc->env != NULL)
			// 	ft_strarrdel(&proc->env);

			proc = proc->next;
		}
	}
	if (job->bg == true)
		jobs_bg_job(job, false);
	else
		g_state->exit_code = jobs_fg_job(job, false);
	signal(SIGCHLD, signal_handle_child_death);
}
