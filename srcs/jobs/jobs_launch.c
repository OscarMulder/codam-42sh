/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   jobs_launch.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: rkuijper <rkuijper@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/10/28 16:25:10 by rkuijper       #+#    #+#                */
/*   Updated: 2019/11/05 11:57:35 by jbrinksm      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"
#include "signal.h"
#include <unistd.h>

static void	setup_fork(t_job *job, t_proc *proc, int fds[3], int pipes[2])
{
	if (proc->pid < 0)
	{
		ft_eprintf("Could not fork.\n");
		exit(1);
	}
	if (proc->pid == 0)
		jobs_launch_proc(job, proc, fds, pipes);
	else
	{
		if (g_state->shell_type == SHELL_INTERACT)
		{
			if (job->pgid == 0)
				job->pgid = proc->pid;
			setpgid(proc->pid, job->pgid);
		}
	}
}

static int	prepare_arguments_proc(t_ast *node, t_proc *proc, char ***argv)
{
	if (expan_handle_variables(node, g_data->envlst) == FUNCT_ERROR)
	{
		//ERROR
		ft_eprintf("ERROR3!\n");
		return (FUNCT_ERROR);
	}
	if (node->type == WORD && expan_pathname(node) == FUNCT_ERROR)
	{
		//ERROR
		ft_eprintf("ERROR4!\n");
		return (FUNCT_ERROR);
	}
	exec_quote_remove(node);
	if (node->type == WORD)
		proc->redir_and_assign = node->right;
	else
		proc->redir_and_assign = node;
	*argv = exec_create_process_args(node);
	if (*argv == NULL)
	{
		//ERROR
		ft_eprintf("ERROR1!\n");
		return (FUNCT_ERROR);
	}
	return (FUNCT_SUCCESS);
}

static int	prepare_settings_proc(t_job *job, t_proc *proc, t_ast *node,
				char **argv)
{
	if (exec_command_contains_only_assign(node) == true)
		proc->no_fork = true;
	if (proc->no_fork == true && exec_assigns(proc->redir_and_assign,
		g_data, ENV_LOCAL) == FUNCT_ERROR)
	{
		//ERROR
		ft_eprintf("ERROR2!\n");
		return (FUNCT_ERROR);
	}
	jobs_update_job_command(job, argv);
	proc->argv = argv;
	if (ft_strequ(proc->argv[0], "") == true)
		proc->no_cmd = true;
	if (exec_builtin(argv, proc) == false)
		exec_external(argv, g_data, proc);
	return (FUNCT_SUCCESS);
}

static int	handle_nonforked_builtin(t_job *job, t_proc *proc)
{
	if (job->bg == false && jobs_exec_is_single_builtin_proc(job->processes))
	{
		jobs_exec_builtin(job->processes);
		env_remove_tmp(g_data->envlst);
		jobs_finished_job(job, true);
		return (FUNCT_FAILURE);
	}
	if (proc->is_builtin == false && proc->binary == NULL)
	{
		jobs_flush_job(job);
		g_state->exit_code = 1;
		return (FUNCT_FAILURE);
	}
	return (FUNCT_SUCCESS);
}

static int	launch_forked_job(t_job *job, int fds[3], int pipes[2])
{
	pid_t	pid;
	t_proc	*proc;
	char	**argv;
	t_ast	*node;

	proc = job->processes;
	while (proc != NULL)
	{
		argv = NULL;
		node = proc->node;
		if (prepare_arguments_proc(node, proc, &argv) == FUNCT_ERROR)
			return (FUNCT_ERROR);
		if (prepare_settings_proc(job, proc, node, argv) == FUNCT_ERROR)
			return (FUNCT_ERROR);
		if (handle_nonforked_builtin(job, proc) != FUNCT_SUCCESS)
			return (FUNCT_ERROR);
		jobs_launch_setup_stds(proc, fds, pipes);
		pid = fork();
		proc->pid = pid;
		setup_fork(job, proc, fds, pipes);
		jobs_launch_cleanup_after_proc(proc, fds, pipes);
		proc = proc->next;
	}
	return (FUNCT_SUCCESS);
}

void		jobs_launch_job(t_job *job)
{
	int		fds[3];
	int		pipes[2];
	int		ret;

	fds[0] = STDIN_FILENO;
	fds[2] = STDERR_FILENO;
	ft_memset(pipes, UNINIT, sizeof(pipes));
	ret = launch_forked_job(job, fds, pipes);
	if (ret == FUNCT_FAILURE || ret == FUNCT_ERROR)
		return ;
	jobs_add_job(g_data, job);
	if (job->bg == true)
		jobs_bg_job(job, false);
	else
		g_state->exit_code = jobs_fg_job(job, false);
}
