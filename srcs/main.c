/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: omulder <omulder@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/04/10 20:29:49 by jbrinksm       #+#    #+#                */
/*   Updated: 2019/04/18 16:45:07 by omulder       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"

/*
**	ft_printf alloc error handling
*/

int		main(int argc, char **argv)
{
	t_term	*term_p;

	(void)argv;
	(void)argc;
	term_p = term_init();
	/* if !term_p: send appropriate error message/log */
	if (!term_p)
		return (EXIT_FAILURE);
	shell_start();
	term_reset(term_p);
	term_free_termp(term_p);
}
