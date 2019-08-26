/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: jbrinksm <jbrinksm@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/04/10 20:29:49 by jbrinksm       #+#    #+#                */
/*   Updated: 2019/08/22 15:36:57 by tde-jong      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"

/*
**	ft_printf alloc error handling
*/

int		main(void)
{
	t_vshdata	data;

	if (isatty(STDIN_FILENO) != 1)
	{
		ft_eprintf(E_STDIN_NOT_TTY);
		return (EXIT_FAILURE);
	}
	g_state = (t_state*)ft_memalloc(sizeof(t_state));
	if (g_state == NULL)
		return (EXIT_FAILURE);
	g_state->exit_code = EXIT_SUCCESS;
	if (shell_init_vshdata(&vshdata) == FUNCT_ERROR)
		return (EXIT_FAILURE);
	if (redir_save_stdfds(&vshdata) == FUNCT_ERROR)
		return (EXIT_FAILURE);
	shell_start(&vshdata);
	return (EXIT_SUCCESS);
}
