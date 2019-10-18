/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   jobs_bg.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: rkuijper <rkuijper@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/10/18 16:46:22 by rkuijper       #+#    #+#                */
/*   Updated: 2019/10/18 17:05:17 by rkuijper      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"
#include <errno.h>

void	jobs_bg_job(t_job *job, bool job_continued)
{
	job->bg = true;
	if (job_continued && kill(-job->pgid, SIGCONT) < 0)
		ft_eprintf("kill (SIGCONT): %e\n", errno);
	return (FUNCT_SUCCESS);
}
