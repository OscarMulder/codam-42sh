/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   signals.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: tde-jong <tde-jong@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/07/22 13:01:13 by tde-jong       #+#    #+#                */
/*   Updated: 2019/07/29 15:07:47 by tde-jong      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"
#include <signal.h>
#include <termios.h>

void	signals_init(void)
{
	// int ret;

	// g_state->termios_p->c_lflag &= ISIG;
	// ret = tcsetattr(STDIN_FILENO, TCSANOW, g_state->termios_p);
	// ft_printf("initializing signals %i\n", ret);
	signal(SIGINT, signals_handle_ctrl_c);
}

void	signals_destroy(void)
{
	// int ret;

	// ft_printf("destroying signals\n");
	// g_state->termios_p->c_lflag &= ~ISIG;
	// ret = tcsetattr(STDIN_FILENO, TCSANOW, g_state->termios_p);
	signal(SIGINT, SIG_DFL);
}
