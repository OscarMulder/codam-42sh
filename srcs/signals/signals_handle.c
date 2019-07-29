/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   signals_handle.c                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: tde-jong <tde-jong@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/07/22 13:04:30 by tde-jong       #+#    #+#                */
/*   Updated: 2019/07/29 13:45:49 by tde-jong      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"
#include <signal.h>

void	signals_handle_ctrl_c(int signum)
{
	(void)signum;
	if (g_state->job_pid > 0)
		ft_printf("^C\n");
}
