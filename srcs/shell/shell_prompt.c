/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   shell_prompt.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: jbrinksm <jbrinksm@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/04/11 20:16:38 by jbrinksm       #+#    #+#                */
/*   Updated: 2019/07/30 17:45:57 by jbrinksm      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"

void	shell_display_prompt(void)
{
	char *current_dir;
	char *arrow;

	if (g_state->exit_code == EXIT_SUCCESS)
		arrow = "\033[1;33m";
	else
		arrow = "\033[1;31m";
	current_dir = shell_getcurrentdir();
	if (current_dir == NULL)
		ft_printf("\033[1;31vsh %s> \033[0m", arrow);
	else
		ft_printf("\033[1;31mvsh \033[1;36m%s %s> \033[0m", current_dir, arrow);
}
