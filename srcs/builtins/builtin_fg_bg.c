/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   builtin_fg_bg.c                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: anonymous <anonymous@student.codam.nl>       +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/09/10 17:16:05 by anonymous      #+#    #+#                */
/*   Updated: 2019/09/10 17:16:05 by anonymous     ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"
#include <unistd.h>
#include <sys/wait.h>
#include <termios.h>
#include <signal.h>

/*
**	The following two functions need a lot of testing and possibly reworking..
*/

static void		term_flags_init(t_termios *termios_p)
{
	termios_p->c_lflag |= (ECHO | ICANON | ISIG);
	tcsetattr(STDIN_FILENO, TCSANOW, termios_p);
}

static void		term_flags_destroy(t_termios *termios_p)
{
	termios_p->c_lflag &= ~(ECHO | ICANON | ISIG);
	tcsetattr(STDIN_FILENO, TCSANOW, termios_p);
}

/* Needs to handle arguments still... */
int		builtin_fg(char **args, t_vshdata* data)
{
	t_job	*job;
	int		status;
	
	(void)args;
	job = data->jobs->joblist;
	if (job == NULL)
		return (err_ret("fg: no current job\n"));
	while (job != NULL)
	{
		ft_eprintf("Checking job with pid: %i\n", job->process_id);
		if (tools_get_pid_state(job->process_id) == PID_SUSPEND)
		{
			signal(SIGINT, signal_print_newline);
			term_flags_init(data->term->termios_p);
			kill(job->process_id, SIGCONT);
			waitpid(job->process_id, &status, WUNTRACED);
			term_flags_destroy(data->term->termios_p);
			return (FUNCT_SUCCESS);
		}
		job = job->next;
	}
	return (FUNCT_SUCCESS);
}

/* Needs to handle arguments still... */
int		builtin_bg(char **args, t_vshdata* data)
{
	t_job	*job;
	int		status;
	
	(void)args;
	job = data->jobs->joblist;
	if (job == NULL)
		return (err_ret("bg: no current job\n"));
	while (job != NULL)
	{
		ft_eprintf("Checking job with pid: %i\n", job->process_id);
		if (tools_get_pid_state(job->process_id) == PID_SUSPEND)
		{
			kill(job->process_id, SIGCONT);
			waitpid(job->process_id, &status, WNOHANG);
			return (FUNCT_SUCCESS);
		}
		job = job->next;
	}
	return (FUNCT_SUCCESS);
}
