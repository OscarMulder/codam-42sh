/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   signal_reset.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: rkuijper <rkuijper@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/10/21 13:44:58 by rkuijper       #+#    #+#                */
/*   Updated: 2019/10/28 12:42:29 by rkuijper      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <signal.h>

void	signal_reset(void)
{
	int i;

	i = 1;
	while (i < 32)
	{
		signal(i, SIG_DFL);
		i++;
	}
}
