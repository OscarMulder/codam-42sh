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
	

	if (args[1] == NULL)
		job = builtin_jobs_find_job("%%", data->jobs->joblist);
	else
		job = builtin_jobs_find_job(args[1], data->jobs->joblist);
	if (job == NULL)
			return (err_ret("fg: no current job\n"));
	job->current = builtin_jobs_new_current_val(data->jobs->joblist);
	print_job_info(job, 0, data->jobs->joblist);
	term_flags_init(data->term->termios_p);
	kill(job->pgid, SIGCONT);
	waitpid(job->pgid, &status, WUNTRACED);
	term_flags_destroy(data->term->termios_p);
	return (FUNCT_SUCCESS);
}
