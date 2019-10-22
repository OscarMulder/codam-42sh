/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   signal_reset.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: rkuijper <rkuijper@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/10/21 13:44:58 by rkuijper       #+#    #+#                */
/*   Updated: 2019/10/21 13:45:34 by rkuijper      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <signal.h>

void	signal_reset(void)
{
	int i;

	i = 0;
	while (i < 33)
	{
		signal(i, SIG_DFL);
		i++;
	}
}
