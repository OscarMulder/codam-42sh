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

int		builtin_fg(char **args, t_vshdata* data)
{
	(void)args;
	(void)data;
	/* If the job list is null, no need to check if there are any active jobs. */
	if (data->jobs->joblist == NULL)
		return (err_ret("fg: no current job\n"));
	return (FUNCT_SUCCESS);
}

int		builtin_bg(char **args, t_vshdata* data)
{
	(void)args;
	(void)data;
	/* If the job list is null, no need to check if there are any active jobs. */
	if (data->jobs->joblist == NULL)
		return (err_ret("fg: no current job\n"));
	return (FUNCT_SUCCESS);
}
