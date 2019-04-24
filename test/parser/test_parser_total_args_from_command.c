/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   test_parser_total_args_from_command.c              :+:    :+:            */
/*                                                     +:+                    */
/*   By: jbrinksm <jbrinksm@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/04/23 16:22:12 by jbrinksm       #+#    #+#                */
/*   Updated: 2019/04/24 20:31:28 by jbrinksm      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"

static int	test_parser_total_args_from_command_easy(void)
{
	int		result;

	result = parser_total_args_from_command\
("simple command line");
	if (result != 3)
		return (FUNCT_FAILURE);
	result = parser_total_args_from_command\
("a lot\t of  args though\twhich   \t might be too many ?");
	if (result != 11)
		return (FUNCT_FAILURE);
	result = parser_total_args_from_command\
("\t\t   \t\t");
	if (result != 1)
		return (FUNCT_FAILURE);
	return (FUNCT_SUCCESS);
}

static int	test_parser_total_args_from_command_moderate(void)
{
	int		result;

	result = parser_total_args_from_command\
("commandlinewith\\ escape" "and quote");
	if (result != 4)
		return (FUNCT_FAILURE);
	result = parser_total_args_from_command\
("command\\;line\\;with;more\\;escaped;semi\\;colons");
	if (result != 3)
		return (FUNCT_FAILURE);
	return (FUNCT_SUCCESS);
}

static int	test_parser_total_args_from_command_hard(void)
{
	int		result;

	result = parser_total_args_from_command\
("commandline;with\";quoted;\"semi;colons");
	if (result != 3)
		return (FUNCT_FAILURE);
	result = parser_total_args_from_command\
("\"nice;weird;\";quoted\"semi;colons\"");
	if (result != 4)
		return (FUNCT_FAILURE);
	return (FUNCT_SUCCESS);
}

int			test_parser_total_args_from_command(void)
{
	int ret;

	ret = test_parser_total_args_from_command_easy();
	if (ret != FUNCT_SUCCESS)
		return (ret);
	ret = test_parser_total_args_from_command_moderate();
	if (ret != FUNCT_SUCCESS)
		return (ret);
	ret = test_parser_total_args_from_command_hard();
	return (ret);
}
