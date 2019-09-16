/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   signal_print_newline.c                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: jbrinksm <jbrinksm@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/09/16 08:14:45 by jbrinksm       #+#    #+#                */
/*   Updated: 2019/09/16 17:12:06 by jbrinksm      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"
#include <signal.h>

void		signal_print_newline(int signum)
{
	if (signum == SIGINT)
	{
		ft_putchar('\n');
		signal(SIGINT, signal_print_newline);
	}
}
