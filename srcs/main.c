/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: jbrinksm <jbrinksm@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/04/10 20:29:49 by jbrinksm       #+#    #+#                */
/*   Updated: 2019/09/24 14:28:54 by mavan-he      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"
#include <signal.h>	

int		main(int argc, char **argv)
{
	t_vshdata	*data;

	signal(SIGCHLD, signal_handle_child_death);
	signal(SIGINT, SIG_IGN);
	g_state = (t_state*)ft_memalloc(sizeof(t_state));
	if (g_state == NULL)
		exit(EXIT_FAILURE);
	g_state->exit_code = EXIT_SUCCESS;
	data = shell_init_vshdata();
	if (data == NULL)
		exit(EXIT_FAILURE);
	if (redir_save_stdfds(data) == FUNCT_ERROR)
		exit(EXIT_FAILURE);
	if (argc > 1 || isatty(STDIN_FILENO) != 1)
	{
		if (argc > 1)
			shell_args(data, argv[1]);
		else
			shell_stdin(data);
		exit(g_state->exit_code);
	}
	if (shell_init_term(data) == FUNCT_ERROR)
		exit(EXIT_FAILURE);
	shell_start(data);
	exit(g_state->exit_code);
}
