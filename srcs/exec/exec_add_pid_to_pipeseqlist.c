/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   exec_add_pid_to_pipeseqlist.c                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: jbrinksm <jbrinksm@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/09/16 07:59:07 by jbrinksm       #+#    #+#                */
/*   Updated: 2019/09/16 07:59:12 by jbrinksm      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"

void		exec_add_pid_to_pipeseqlist(t_vshdata *data, pid_t pid)
{
	t_pipeseqlist	*probe;

	if (data->pipeseq == NULL)
	{
		data->pipeseq = ft_memalloc(sizeof(t_pipeseqlist));
		if (data->pipeseq == NULL) //WHAT DO WE DO ON FAIL
			return ;
		data->pipeseq->pid = pid;
	}
	else
	{
		probe = data->pipeseq;
		data->pipeseq = ft_memalloc(sizeof(t_pipeseqlist));
		if (data->pipeseq == NULL) //WHAT DO WE DO ON FAIL
			return ;
		data->pipeseq->pid = pid;
		data->pipeseq->next = probe;
	}
}
