/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: jbrinksm <jbrinksm@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/04/10 20:29:49 by jbrinksm       #+#    #+#                */
/*   Updated: 2019/07/15 13:02:43 by omulder       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"

/*
**	ft_printf alloc error handling
*/

int		main(int argc, char **argv)
{
	t_term		*term_p;
	t_envlst	*envlst;
	t_history	**history;

	(void)argv;
	(void)argc;
	envlst = env_getlst();
	if (envlst == NULL)
		return (EXIT_FAILURE);
	term_p = term_prepare(envlst);
	history_get_file_content(&history);
	/* if !term_p or history failed: send appropriate error message/log */
	if (term_p == NULL)
		return (EXIT_FAILURE);
	shell_start(envlst);
	if (term_reset(term_p) == FUNCT_FAILURE)
		return (EXIT_FAILURE);
	term_free_struct(&term_p);
	return (EXIT_SUCCESS);
}
