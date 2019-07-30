/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: jbrinksm <jbrinksm@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/04/10 20:29:49 by jbrinksm       #+#    #+#                */
/*   Updated: 2019/07/28 17:12:33 by omulder       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"

static int	redir_save_stdfds(t_vshdata *vshdata)
{
	vshdata->stdfds[STDIN_FILENO] = dup(STDIN_FILENO);
	if (vshdata->stdfds[STDIN_FILENO] == -1)
		return (FUNCT_ERROR);
	vshdata->stdfds[STDOUT_FILENO] = dup(STDOUT_FILENO);
	if (vshdata->stdfds[STDOUT_FILENO] == -1)
		return (FUNCT_ERROR);
	vshdata->stdfds[STDERR_FILENO] = dup(STDERR_FILENO);
	if (vshdata->stdfds[STDERR_FILENO] == -1)
		return (FUNCT_ERROR);
	return (FUNCT_SUCCESS);
}

/*
**	ft_printf alloc error handling
*/

int		main(int argc, char **argv)
{
	t_term		*term_p;
	t_vshdata	vshdata;

	(void)argv;
	(void)argc;
	g_state = (t_state*)ft_memalloc(sizeof(t_state));
	if (g_state == NULL)
		return (EXIT_FAILURE);
	vshdata.aliaslst = NULL;
	vshdata.envlst = env_getlst();
	if (vshdata.envlst == NULL)
		return (EXIT_FAILURE);
	term_p = term_prepare(vshdata.envlst);
	vshdata.history_file = history_find_histfile(&vshdata);
	history_get_file_content(&vshdata);
	/* if !term_p, history or envlst failed: send appropriate error message/log */
	if (term_p == NULL)
		return (EXIT_FAILURE);
	if (redir_save_stdfds(&vshdata) == FUNCT_ERROR)
		return (EXIT_FAILURE);
	shell_start(&vshdata);
	if (term_reset(term_p) == FUNCT_FAILURE)
		return (EXIT_FAILURE);
	term_free_struct(&term_p);
	return (EXIT_SUCCESS);
}
