/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   exec_builtin.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: omulder <omulder@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/05/29 16:59:41 by omulder        #+#    #+#                */
/*   Updated: 2019/10/22 14:36:45 by rkuijper      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"

static bool		exec_builtin_cont(char **args, t_vshdata *data)
{
	if (ft_strequ(args[0], "history"))
		history_print(data->history->history);
	else if (ft_strequ(args[0], "type"))
		builtin_type(args, data->envlst, data->alias->aliaslst);
	else if (ft_strequ(args[0], "alias"))
		builtin_alias(args, &data->alias->aliaslst);
	else if (ft_strequ(args[0], "unalias"))
		builtin_unalias(args, &data->alias->aliaslst);
	else if (ft_strequ(args[0], "jobs"))
		builtin_jobs(args, data);
	else if (ft_strequ(args[0], "fg"))
		builtin_fg(args, data);
	else if (ft_strequ(args[0], "bg"))
		builtin_bg(args, data);
	else if (ft_strequ(args[0], "hash"))
		builtin_hash(args, data);
	else
		return (false);
	return (true);
}

bool		exec_builtin(char **args, t_vshdata *data)
{
	if (ft_strequ(args[0], "echo"))
		builtin_echo(args);
	else if (ft_strequ(args[0], "exit"))
		builtin_exit(args, data);
	else if (ft_strequ(args[0], "cd"))
		builtin_cd(args, data);
	else if (ft_strequ(args[0], "fc"))
		builtin_fc(args, data);
	else if (ft_strequ(args[0], "export"))
		builtin_export(args, data);
	else if (ft_strequ(args[0], "set"))
		builtin_set(args, data->envlst);
	else if (ft_strequ(args[0], "unset"))
		builtin_unset(args, data->envlst);
	else if (ft_strequ(args[0], "type"))
		builtin_type(args, data->envlst, data->alias->aliaslst);
	else if (ft_strequ(args[0], "alias"))
		builtin_alias(args, &data->alias->aliaslst);
	else if (ft_strequ(args[0], "unalias"))
		builtin_unalias(args, &data->alias->aliaslst);
	else
		return (exec_builtin_cont(args, data));
	return (true);
}

/*

static void		exec_child(t_job *job, char *binary, char **args, char **env)
{
	signal_reset();
	setpgid(0, job->pgid);
	execve(binary, args, env);
	ft_eprintf(E_FAIL_EXEC_P, binary);
	exit(EXIT_FAILURE);
}

static void		exec_parent(t_job *job, pid_t pid)
{
	if (job != NULL && job->pgid == 0)
		job->pgid = pid;
	setpgid(pid, job->pgid);
	jobs_add_process(job, pid);
	if (g_data->exec_flags & EXEC_WAIT)
	{
		if (g_data->exec_flags & EXEC_BG)
		{
			g_data->exec_flags &= ~EXEC_BG;
			jobs_bg_job(job, false);
		}
		else
			g_state->exit_code = jobs_fg_job(job, false);
	}
}

static void		exec_bin(char *binary, char **args, char **env,
t_vshdata *data)
{
	pid_t	pid;
	t_job	*job;
	void	*old_sig;

	old_sig = signal(SIGCHLD, SIG_IGN);
	if (exec_validate_binary(binary) == FUNCT_ERROR)
		return ;
	job = jobs_find_current_job(data->jobs->joblist);
	pid = fork();
	if (pid < 0)
		return (err_void_exit(E_FORK_STR, EXIT_FAILURE));
	if (pid == 0)
		exec_child(job, binary, args, env);
	exec_parent(job, pid);
	signal(SIGCHLD, old_sig);
}*/
