/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   jobs_cont.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: rkuijper <rkuijper@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/10/18 16:47:14 by rkuijper       #+#    #+#                */
/*   Updated: 2019/10/18 17:12:36 by rkuijper      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"

void		jobs_continue_job(t_job *job, bool fg)
{
	jobs_print_job_info(job, JOB_OPT_P, g_data->jobs);
	if (fg == true)
		jobs_job_fg(job, true);
	else
		jobs_job_bg(job, true);
}
