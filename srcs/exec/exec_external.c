/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   exec_external.c                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: omulder <omulder@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/05/31 10:47:19 by tde-jong       #+#    #+#                */
/*   Updated: 2019/10/28 16:58:39 by jbrinksm      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"
#include <signal.h>
#include <unistd.h>
#include <termios.h>
#include <sys/wait.h>

// static void		exec_child(t_job *job, char *binary, char **args, char **env)
// {
// 	if (g_state->shell_type == SHELL_INTERACT)
// 	{
// 		setpgid(0, job->pgid);
// 		if (!(g_data->exec_flags & EXEC_BG))
// 			tcsetpgrp(STDIN_FILENO, job->pgid == 0 ? getpid() : job->pgid);
// 		signal_reset();
// 	}
// 	execve(binary, args, env);
// 	ft_eprintf(E_FAIL_EXEC_P, binary);
// 	exit(EXIT_FAILURE);
// }

// static void		exec_parent(t_job *job, pid_t pid)
// {
// 	if (g_state->shell_type == SHELL_INTERACT)
// 	{
// 		if (job->pgid == 0)
// 			job->pgid = pid;
// 		setpgid(pid, job->pgid);
// 	}
// 	if (g_data->exec_flags & EXEC_WAIT)
// 	{
// 		if (g_data->exec_flags & EXEC_BG)
// 			jobs_bg_job(job, false);
// 		else
// 			g_state->exit_code = jobs_fg_job(job, false);
// 	}
// }

static void		exec_bin(char *binary, char **args, char **env,
t_vshdata *data)
{
	t_job	*job;

	//old_sig = signal(SIGCHLD, SIG_DFL);
	if (exec_validate_binary(binary) == FUNCT_ERROR)
		return ;
	job = data->jobs->active_job;
	if (job == NULL)
	{
		job = jobs_add_job(data, 0, "");
		data->jobs->active_job = job;
	}
	if (job == NULL)
		return ;
	jobs_update_job_command(job, args);
	jobs_add_process(job);
	if (job->last_proc == NULL)
		exit(1);
	job->last_proc->env = env;
	job->last_proc->argv = args;
	job->last_proc->binary = binary;
	if (g_data->exec_flags & EXEC_BG)
		job->bg = true;
	else
		job->bg = false;
	
	/*pid = fork();
	if (pid < 0)
		return (err_void_exit(E_FORK_STR, EXIT_FAILURE));
	if (pid == 0)
		exec_child(job, binary, args, env);
	exec_parent(job, pid);
	signal(SIGCHLD, old_sig);*/
}

void			exec_external(char **args, t_vshdata *data)
{
	char	**vshenviron;
	char	*binary;

	binary = ft_strdup(args[0]);
	vshenviron = env_lsttoarr(data->envlst);
	if (binary == NULL || vshenviron == NULL)
	{
		ft_strdel(&binary);
		free(vshenviron);
		ft_eprintf(E_ALLOC_STR);
		g_state->exit_code = EXIT_FAILURE;
		return ;
	}
	if (args[0][0] != '/' && ft_strnequ(args[0], "./", 2) == 0 &&
		ft_strnequ(args[0], "../", 3) == 0)
	{
		ft_strdel(&binary);
		if (exec_find_binary(args[0], data, &binary) == FUNCT_SUCCESS)
			exec_bin(binary, args, vshenviron, data);
	}
	else
		exec_bin(binary, args, vshenviron, data);
}
