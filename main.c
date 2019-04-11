/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: jbrinksm <jbrinksm@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/04/10 20:29:49 by jbrinksm       #+#    #+#                */
/*   Updated: 2019/04/11 14:19:20 by jbrinksm      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"

int		reset_term(t_term *term_p)
{
	int	ret;

	/* Is TCSANOW flag correct? */
	ret = tcsetattr(STDIN_FILENO, TCSANOW, term_p->old_termios_p);
	if (ret == -1)
		return (FUNCT_SUCCESS);
	return (FUNCT_SUCCESS);
}

void	free_term_p(t_term *term_p)
{
	if (term_p)
	{
		if (term_p->old_termios_p)
			free(term_p->old_termios_p);
		if (term_p->termios_p)
			free(term_p->termios_p);
		free(term_p);
	}
}

/*
**	ft_printf alloc error handling
*/

int		main(int argc, char **argv)
{
	t_term	*term_p;

	(void)argv;
	(void)argc;
	term_p = init_term();
	/* if !term_p: send appropriate error message/log */
	if (!term_p)
		return (EXIT_FAILURE);
	start_shell(term_p);
	reset_term(term_p);
}
