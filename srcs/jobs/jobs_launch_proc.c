/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   jobs_launch_proc.c                                 :+:    :+:            */
/*                                                     +:+                    */
/*   By: rkuijper <rkuijper@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/10/29 11:20:31 by rkuijper       #+#    #+#                */
/*   Updated: 2019/10/30 16:08:15 by rkuijper      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"

static void	execute_builtin_continued(t_proc *proc)
{
	if (ft_strequ(proc->argv[0], "alias"))
		builtin_alias(proc->argv, &g_data->alias->aliaslst);
	else if (ft_strequ(proc->argv[0], "unalias"))
		builtin_unalias(proc->argv, &g_data->alias->aliaslst);
	else if (ft_strequ(proc->argv[0], "jobs"))
		builtin_jobs(proc->argv, g_data);
	else if (ft_strequ(proc->argv[0], "fg"))
		builtin_fg(proc->argv, g_data);
	else if (ft_strequ(proc->argv[0], "bg"))
		builtin_bg(proc->argv, g_data);
	else if (ft_strequ(proc->argv[0], "hash"))
		builtin_hash(proc->argv, g_data);
}

void		jobs_execute_builtin(t_proc *proc, bool do_exit)
{
	if (ft_strequ(proc->argv[0], "echo"))
		builtin_echo(proc->argv);
	else if (ft_strequ(proc->argv[0], "exit"))
		builtin_exit(proc->argv, g_data);
	else if (ft_strequ(proc->argv[0], "cd"))
		builtin_cd(proc->argv, g_data);
	else if (ft_strequ(proc->argv[0], "fc"))
		builtin_fc(proc->argv, g_data);
	else if (ft_strequ(proc->argv[0], "export"))
		builtin_export(proc->argv, g_data);
	else if (ft_strequ(proc->argv[0], "set"))
		builtin_set(proc->argv, g_data->envlst);
	else if (ft_strequ(proc->argv[0], "unset"))
		builtin_unset(proc->argv, g_data->envlst);
	else if (ft_strequ(proc->argv[0], "history"))
		history_print(g_data->history);
	else if (ft_strequ(proc->argv[0], "type"))
		builtin_type(proc->argv, g_data->envlst, g_data->alias->aliaslst);
	else
		execute_builtin_continued(proc);
	if (do_exit == true)
		exit(g_state->exit_code);
}

static void	handle_filedescriptors(int fds[3], int pipes[2])
{
	if (fds[0] != STDIN_FILENO)
	{
		dup2(fds[0], STDIN_FILENO);
		close(fds[0]);
	}
	else if (pipes[0] != UNINIT)
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

static void	execute_proc(t_proc *proc)
{
	if (proc->no_cmd == true)
		exit(0);
	if (proc->is_builtin == false)
	{
		execve(proc->binary, proc->argv, proc->env);
		ft_eprintf(E_BIN_PROC_LAUNCH, proc->binary);
		exit(1);
	}
	jobs_execute_builtin(proc, true);
}

void		jobs_launch_proc(t_job *job, t_proc *proc, int fds[3], int pipes[2])
{
	pid_t	pid;

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
	if (exec_assigns(proc->redir_and_assign, g_data, ENV_TEMP) == FUNCT_ERROR)
		return ;
	handle_filedescriptors(fds, pipes);
	if (exec_redirs(proc->redir_and_assign) == FUNCT_ERROR)
		return ;
	proc->env = env_lsttoarr(g_data->envlst);
	if (proc->env == NULL)
	{
		ft_eprintf(E_ALLOC_STR);
		exit(1);
	}
	execute_proc(proc);
}
