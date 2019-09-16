/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   signal_print_newline.c                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: jbrinksm <jbrinksm@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/09/16 08:14:45 by jbrinksm       #+#    #+#                */
/*   Updated: 2019/09/16 10:10:01 by jbrinksm      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"

void		signal_print_newline(int signum)
{
	(void)signum;
	ft_putchar('%');
	ft_putchar('\n');
	signal(SIGINT, signal_print_newline);
}
