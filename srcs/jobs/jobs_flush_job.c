/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   jobs_flush_job.c                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: rkuijper <rkuijper@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/10/31 10:59:38 by rkuijper       #+#    #+#                */
/*   Updated: 2019/11/04 10:49:44 by rkuijper      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"

void			jobs_flush_job(t_job *job, bool flush_children)
{
	jobs_flush_process(job->processes);
	if (job->command != NULL)
		ft_strdel(&job->command);
	if (flush_children == true && job->child != NULL)
		jobs_flush_job(job->child, flush_children);
	ft_memdel((void**)&job);
}
