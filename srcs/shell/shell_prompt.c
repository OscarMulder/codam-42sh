/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   shell_prompt.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: jbrinksm <jbrinksm@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/04/11 20:16:38 by jbrinksm       #+#    #+#                */
/*   Updated: 2019/07/30 18:24:22 by jbrinksm      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"
#include <unistd.h>
#include <sys/param.h>

void	shell_display_prompt(void)
{
	char *cwd;
	char *lastdir;
	char *arrow;

	cwd = getcwd(NULL, MAXPATHLEN);
	if (g_state->exit_code == EXIT_SUCCESS)
		arrow = "\033[1;33m";
	else
		arrow = "\033[1;31m";
	lastdir = shell_getcurrentdir(cwd);
	if (lastdir == NULL)
		ft_printf("\033[1;31vsh %s> \033[0m", arrow);
	else
		ft_printf("\033[1;31mvsh \033[1;36m%s %s> \033[0m", lastdir, arrow);
	ft_strdel(&cwd);
}
