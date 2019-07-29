/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   signals_handle.c                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: tde-jong <tde-jong@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/07/22 13:04:30 by tde-jong       #+#    #+#                */
/*   Updated: 2019/07/29 13:00:03 by tde-jong      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"
#include <signal.h>

void	signals_handle_ctrl_c(int signum)
{
	if (signum == SIGINT)
	{
		ft_printf("caught ctrl+c\n");
	}
}
