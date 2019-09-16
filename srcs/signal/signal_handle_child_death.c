/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   signal_handle_child_death.c                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: jbrinksm <jbrinksm@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/09/16 09:51:07 by jbrinksm       #+#    #+#                */
/*   Updated: 2019/09/16 10:53:35 by jbrinksm      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"
#include <signal.h>

void		signal_handle_child_death(int placeholder)
{
	t_pipeseqlist	*pipeseqprobe;
	bool			killmode;

	(void)placeholder;
	pipeseqprobe = g_data->pipeseq;
	killmode = false;
	while (pipeseqprobe != NULL)
	{
		if (killmode == true)
			kill(pipeseqprobe->pid, SIGINT);
		if (tools_get_pid_state(pipeseqprobe->pid) == PID_STATE_EXIT)
			killmode = true;
		pipeseqprobe = pipeseqprobe->next;
	}
	signal(SIGCHLD, signal_handle_child_death);
}
