/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: jbrinksm <jbrinksm@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/04/10 20:29:49 by jbrinksm       #+#    #+#                */
/*   Updated: 2019/10/18 17:01:20 by rkuijper      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"
#include <signal.h>

void			signal_handle(int sig)
{
	t_job *job;

	if (sig == SIGTSTP)
	{
		job = g_data->jobs->joblist;
		while (job != NULL)
		{
			kill(job->pgid, SIGTSTP);
			job = job->next;
		}
		ft_putchar('\n');
	}
}

void			sigchld_handle(int sig)
{
	t_job	*job;
	t_job	*prev;

	if (sig != SIGCHLD)
		return ;
	prev = NULL;
	job = g_data->jobs->joblist;
	while (job != NULL)
	{
		if (jobs_get_job_state(job) == JOB_EXIT)
		{
			if (prev == NULL)
				g_data->jobs->joblist = job->next;
			else
				prev->next = job->next;
			ft_strdel(&job->command);
			free(job);
			job = g_data->jobs->joblist;
			continue;
		}
		prev = job;
		job = job->next;
	}
	signal(SIGCHLD, sigchld_handle);
}

void	init_g_state(void)
{
	g_state = (t_state*)ft_memalloc(sizeof(t_state));
	if (g_state == NULL)
	{
		ft_eprintf(E_ALLOC_STR);
		exit(EXIT_FAILURE);
	}
	g_state->shell_type = SHELL_INTERACT;
}

int		main(int argc, char **argv)
{
	t_vshdata	*data;

	signal(SIGTSTP, signal_handle);
	signal(SIGCHLD, sigchld_handle);
	signal(SIGCHLD, signal_handle_child_death);
	signal(SIGINT, SIG_IGN);
	init_g_state();
	data = shell_init_vshdata();
	if (data == NULL)
		return (EXIT_FAILURE);
	if (redir_save_stdfds(data) == FUNCT_ERROR)
		return (EXIT_FAILURE);
	if (argc > 1 || isatty(STDIN_FILENO) != 1)
	{
		g_state->shell_type = SHELL_NON_INTERACT;
		if (argc > 1)
			shell_args(data, argv[1]);
		else
			shell_stdin(data);
		return (g_state->exit_code);
	}
	if (shell_init_term(data) == FUNCT_ERROR)
		return (EXIT_FAILURE);
	shell_start(data);
	return (g_state->exit_code);
}
