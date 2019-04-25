/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   test_parser_strdup_arg_from_command.c              :+:    :+:            */
/*                                                     +:+                    */
/*   By: jbrinksm <jbrinksm@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/04/23 14:52:01 by jbrinksm       #+#    #+#                */
/*   Updated: 2019/04/25 20:18:15 by jbrinksm      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"

static int	return_and_free(char *result, int ret)
{
	ft_strdel(&result);
	return (ret);
}

static int	test_parser_strdup_arg_from_command_easy(void)
{
	char	*result;
	int		index;

	index = 0;
	result = parser_strdup_arg_from_command("simple command line", &index);
	if (result == NULL)
		return (E_ALLOC);
	if (ft_strcmp(result, "simple") != 0)
		return (return_and_free(result, FUNCT_FAILURE));
	ft_strdel(&result);
	index = 8;
	result = parser_strdup_arg_from_command("another command line", &index);
	if (result == NULL)
		return (E_ALLOC);
	if (ft_strcmp(result, "command") != 0)
		return (return_and_free(result, FUNCT_FAILURE));
	ft_strdel(&result);
	index = 16;
	result = parser_strdup_arg_from_command("another command line", &index);
	if (result == NULL)
		return (E_ALLOC);
	if (ft_strcmp(result, "line") != 0)
		return (return_and_free(result, FUNCT_FAILURE));
	return (return_and_free(result, FUNCT_SUCCESS));
}

static int	test_parser_strdup_arg_from_command_moderate(void)
{
	char	*result;
	int		index;

	index = 0;
	result = parser_strdup_arg_from_command("lala\\ lala andmore", &index);
	if (result == NULL)
		return (E_ALLOC);
	if (ft_strcmp(result, "lala\\ lala") != 0)
		return (return_and_free(result, FUNCT_FAILURE));
	ft_strdel(&result);
	index = 0;
	result = parser_strdup_arg_from_command("word\\ \\\ts", &index);
	if (result == NULL)
		return (E_ALLOC);
	if (ft_strcmp(result, "word\\ \\\ts") != 0)
		return (return_and_free(result, FUNCT_FAILURE));
	return (return_and_free(result, FUNCT_SUCCESS));
}

static int	test_parser_strdup_arg_from_command_hard(void)
{
	char	*result;
	int		index;

	index = 0;
	result = parser_strdup_arg_from_command("lala\\ \t and\\ \\\tmore", &index);
	if (result == NULL)
		return (E_ALLOC);
	if (ft_strcmp(result, "lala\\ ") != 0)
		return (return_and_free(result, FUNCT_FAILURE));
	ft_strdel(&result);
	index = 9;
	result = parser_strdup_arg_from_command("01234567 \\ \\ \\ \\ \\\tword\\   "
	"     ", &index);
	if (result == NULL)
		return (E_ALLOC);
	if (ft_strcmp(result, "\\ \\ \\ \\ \\\tword\\ ") != 0)
		return (return_and_free(result, FUNCT_FAILURE));
	return (return_and_free(result, FUNCT_SUCCESS));
}

int			test_parser_strdup_arg_from_command(void)
{
	int ret;

	ret = test_parser_strdup_arg_from_command_easy();
	if (ret != FUNCT_SUCCESS)
		return (ret);
	ret = test_parser_strdup_arg_from_command_moderate();
	if (ret != FUNCT_SUCCESS)
		return (ret);
	ret = test_parser_strdup_arg_from_command_hard();
	return (ret);
}
