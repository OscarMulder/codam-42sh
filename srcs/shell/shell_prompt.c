/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   shell_prompt.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: jbrinksm <jbrinksm@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/04/11 20:16:38 by jbrinksm       #+#    #+#                */
/*   Updated: 2019/08/06 11:24:26 by jbrinksm      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"

void	shell_display_prompt(t_vshdata *vshdata)
{
	char *cwd;
	char *lastdir;
	char *arrow;

	cwd = env_getvalue("PWD", vshdata->envlst);
	if (g_state->exit_code == EXIT_SUCCESS)
		arrow = YEL "> ";
	else
		arrow = RED "> ";
	lastdir = shell_getcurrentdir(cwd);
	vshdata->prompt_len = 6;
	// if (lastdir == NULL)
		ft_printf(RED "vsh %s" RESET, arrow);
	// else
	// {
	// 	ft_printf(RED "vsh " BLU "%s %s" RESET, lastdir, arrow);
	// 	vshdata->prompt_len = 6 + ft_strlen(lastdir);
	// }
}
