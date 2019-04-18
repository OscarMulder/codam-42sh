/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   test_main.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: tde-jong <tde-jong@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/04/18 16:37:32 by omulder        #+#    #+#                */
/*   Updated: 2019/04/18 21:50:21 by tde-jong      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"

int		test_prompt(void)
{
	shell_display_prompt();
	return (0);
}

int		main(void)
{
	if (test_prompt() == FUNCT_FAILURE)
		return (EXIT_FAILURE);
	if (test_term_init_struct() == FUNCT_FAILURE)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}
