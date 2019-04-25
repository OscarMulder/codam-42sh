/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   test_parser_arg_len_from_command.c                 :+:    :+:            */
/*                                                     +:+                    */
/*   By: jbrinksm <jbrinksm@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/04/23 15:29:24 by jbrinksm       #+#    #+#                */
/*   Updated: 2019/04/25 14:12:25 by jbrinksm      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"

static int	test_parser_arg_len_from_command_easy(void)
{
	int		result;
	int		index;

	index = 0;
	result = parser_command_len_from_line("123456789", &index);
	if (result != 9)
		return (FUNCT_FAILURE);
	result = parser_command_len_from_line("123", &index);
	if (result != 3)
		return (FUNCT_FAILURE);
	result = parser_command_len_from_line("", &index);
	if (result != 0)
		return (FUNCT_FAILURE);
	return (FUNCT_SUCCESS);
}

static int	test_parser_arg_len_from_command_moderate(void)
{
	int		result;
	int		index;

	index = 0;
	result = parser_command_len_from_line("12345 6789", &index);
	if (result != 5)
		return (FUNCT_FAILURE);
	result = parser_command_len_from_line("\"12345 6789\"", &index);
	if (result != 10)
		return (FUNCT_FAILURE);
	result = parser_command_len_from_line("123\t", &index);
	if (result != 3)
		return (FUNCT_FAILURE);
	result = parser_command_len_from_line("\t", &index);
	if (result != 0)
		return (FUNCT_FAILURE);
	return (FUNCT_SUCCESS);
}

static int	test_parser_arg_len_from_command_hard(void)
{
	int		result;
	int		index;

	index = 0;
	result = parser_command_len_from_line("\"1234\"5 6789\"", &index);
	if (result != 5)
		return (FUNCT_FAILURE);
	result = parser_command_len_from_line("123\t", &index);
	if (result != 3)
		return (FUNCT_FAILURE);
	result = parser_command_len_from_line("\\ \t", &index);
	if (result != 2)
		return (FUNCT_FAILURE);
	return (FUNCT_SUCCESS);
}

int			test_parser_arg_len_from_command(void)
{
	int ret;

	ret = test_parser_arg_len_from_command_easy();
	if (ret != FUNCT_SUCCESS)
		return (ret);
	ret = test_parser_arg_len_from_command_moderate();
	if (ret != FUNCT_SUCCESS)
		return (ret);
	ret = test_parser_arg_len_from_command_hard();
	return (ret);
}
