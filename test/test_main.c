/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   test_main.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: jbrinksm <jbrinksm@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/04/18 16:37:32 by omulder        #+#    #+#                */
/*   Updated: 2019/04/23 16:59:41 by jbrinksm      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"

int		main(void)
{
	if (test_prompt() == FUNCT_FAILURE)
		return (EXIT_FAILURE);
	if (test_get_environ_cpy() == FUNCT_FAILURE)
		return (EXIT_FAILURE);
	if (test_param_to_env() == FUNCT_FAILURE)
		return (EXIT_FAILURE);
	if (test_term_is_valid() == FUNCT_FAILURE)
		return (EXIT_FAILURE);
	if (test_term_init_struct() == FUNCT_FAILURE)
		return (EXIT_FAILURE);
	if (test_term_free_struct() == FUNCT_FAILURE)
		return (EXIT_FAILURE);
	if (test_term_get_attributes() == FUNCT_FAILURE)
		return (EXIT_FAILURE);
	if (test_parser_split_commands() == FUNCT_FAILURE)
		return (EXIT_FAILURE);
	if (test_parser_strdup_command_from_line() == FUNCT_FAILURE)
		return (EXIT_FAILURE);
	if (test_parser_command_len_from_line() == FUNCT_FAILURE)
		return (EXIT_FAILURE);
	if (test_parser_total_commands_from_line() == FUNCT_FAILURE)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}
