/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   term_set_attributes.c                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: omulder <omulder@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/04/18 18:11:05 by jbrinksm       #+#    #+#                */
/*   Updated: 2019/08/23 14:27:05 by jbrinksm      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"
#include <termios.h>
#include <unistd.h>

int		term_set_attributes(t_term *term_p)
{
	int	ret;

	term_p->termios_p->c_lflag &= ~(ECHO | ICANON | ISIG);
	term_p->termios_p->c_cc[VMIN] = 0;
	term_p->termios_p->c_cc[VTIME] = 1;
	ret = tcsetattr(STDIN_FILENO, TCSANOW, term_p->termios_p);
	if (ret == -1)
	{
		ft_eprintf(E_TERM_CNT_GET);
		return (FUNCT_FAILURE);
	}
	return (FUNCT_SUCCESS);
}
