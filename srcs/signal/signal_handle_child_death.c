/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   signal_handle_child_death.c                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: jbrinksm <jbrinksm@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/09/16 09:51:07 by jbrinksm       #+#    #+#                */
/*   Updated: 2019/10/21 16:36:54 by rkuijper      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"
#include <signal.h>

void		signal_handle_child_death(int signum)
{
	if (signum == SIGCHLD)
		jobs_notify_pool();
	signal(SIGCHLD, signal_handle_child_death);
}
