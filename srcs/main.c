/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: jbrinksm <jbrinksm@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/04/10 20:29:49 by jbrinksm       #+#    #+#                */
/*   Updated: 2019/09/04 11:25:01 by mavan-he      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"
#include <signal.h>

/*
**	ft_printf alloc error handling
*/

void			signal_handle(int sig)
{
	t_job *job;

	if (sig == SIGSTOP || sig == SIGTSTP)
	{
		job = g_vshdata->jobs->joblist;
		while (job != NULL)
		{
			kill(job->process_id, SIGSTOP);
			job = job->next;
		}
		signal(SIGSTOP, signal_handle);
		signal(SIGTSTP, signal_handle);
	}
}

void			sigchld_handle(int sig)
{
	t_job	*job;
	t_job	*prev;

	if (sig != SIGCHLD)
		return ;
	prev = NULL;
	job = g_vshdata->jobs->joblist;
	while (job != NULL)
	{
		if (jobs_get_job_state(job) == JOB_EXIT)
		{
			if (prev == NULL)
				g_vshdata->jobs->joblist = job->next;
			else
				prev->next = job->next;
			ft_strdel(&job->command_name);
			free(job);
			job = g_vshdata->jobs->joblist;
			continue;
		}
		prev = job;
		job = job->next;
	}
	signal(SIGCHLD, sigchld_handle);
}

int		main(void)
{
	t_vshdata	*data;

	signal(SIGSTOP, signal_handle);
	signal(SIGTSTP, signal_handle);
	signal(SIGCHLD, sigchld_handle);
	if (isatty(STDIN_FILENO) != 1)
	{
		ft_eprintf(E_STDIN_NOT_TTY);
		return (EXIT_FAILURE);
	}
	g_state = (t_state*)ft_memalloc(sizeof(t_state));
	if (g_state == NULL)
		return (EXIT_FAILURE);
	g_state->exit_code = EXIT_SUCCESS;
	data = shell_init_vshdata();
	if (data == NULL)
		return (EXIT_FAILURE);
	if (redir_save_stdfds(data) == FUNCT_ERROR)
		return (EXIT_FAILURE);
	input_resize_window_check(data);
	shell_start(data);
	return (EXIT_SUCCESS);
}
