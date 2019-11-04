/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   jobs_launch.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: rkuijper <rkuijper@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/10/28 16:25:10 by rkuijper       #+#    #+#                */
/*   Updated: 2019/11/04 17:54:30 by jbrinksm      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"
#include "signal.h"
#include <unistd.h>

static size_t	count_args(t_ast *ast)
{
	t_ast	*probe;
	size_t	i;

	i = 0;
	if (ast == NULL || ast->type != WORD)
		return (0);
	probe = ast;
	while (probe != NULL && (probe->type == WORD || probe->type == ASSIGN))
	{
		i++;
		probe = probe->left;
	}
	return (i);
}

static char		**malloc_args(int total_args)
{
	char	**args;

	if (total_args > 0)
		args = (char**)ft_memalloc(sizeof(char*) * (total_args + 1));
	else
	{
		args = (char**)ft_memalloc(sizeof(char*) * 2);
		if (args == NULL)
			return (NULL);
		*args = ft_strnew(0);
		if (*args == NULL)
			ft_strarrdel(&args);
	}
	return (args);
}

static char		**create_args(t_ast *ast)
{
	char	**args;
	t_ast	*probe;
	size_t	total_args;
	size_t	i;

	total_args = count_args(ast);
	args = malloc_args(total_args);
	if (args == NULL)
		return (NULL);
	i = 0;
	probe = ast;
	while (i < total_args)
	{
		args[i] = ft_strdup(probe->value);
		if (args[i] == NULL)
		{
			ft_strarrdel(&args);
			return (NULL);
		}
		probe = probe->left;
		i++;
	}
	return (args);
}

static void	setup_stds(t_proc *proc, int fds[3], int pipes[2])
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
}

static void	cleanup_after_proc(t_proc *proc, int fds[3], int pipes[2])
{
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
}

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

static bool		exec_command_contains_only_assign(t_ast *ast)
{
	t_ast	*probe;

	if (ast->type == WORD)
		return (false);
	probe = ast;
	while (probe != NULL)
	{
		if (tool_is_redirect_tk(probe->type) == true)
			return (false);
		probe = probe->left;
	}
	return (true);
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

		/* */
		node = proc->node;
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
		argv = create_args(node);
		if (argv == NULL)
		{
			//ERROR
			ft_eprintf("ERROR1!\n");
			return (FUNCT_ERROR);
		}
		if (exec_command_contains_only_assign(node) == true)
			proc->no_fork = true;
		if (proc->no_fork == true)
		{
			if (exec_assigns(proc->redir_and_assign, g_data, ENV_LOCAL)
				== FUNCT_ERROR)
			{
				//ERROR
				ft_eprintf("ERROR2!\n");
				return (FUNCT_ERROR);
			}
		}
		jobs_update_job_command(job, argv);
		proc->argv = argv;
		if (ft_strequ(proc->argv[0], "") == true)
			proc->no_cmd = true;
		if (exec_builtin(argv, proc) == false)
			exec_external(argv, g_data, proc);
		/* */

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
		setup_stds(proc, fds, pipes);
		pid = fork();
		proc->pid = pid;
		setup_fork(job, proc, fds, pipes);
		cleanup_after_proc(proc, fds, pipes);
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
