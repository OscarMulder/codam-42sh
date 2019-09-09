/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   exec_get_pid_ptr.c                                 :+:    :+:            */
/*                                                     +:+                    */
/*   By: jbrinksm <jbrinksm@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/09/09 19:27:11 by jbrinksm       #+#    #+#                */
/*   Updated: 2019/09/09 19:40:58 by jbrinksm      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"

pid_t	*exec_get_pid_ptr(pid_t pid)
{
	pid_t	*stored_pid;

    stored_pid = ft_memalloc(sizeof(pid_t));
    if (stored_pid != NULL)
        *stored_pid = pid;
	return (stored_pid);
}
