/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   test_parser_command_len.c                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: jbrinksm <jbrinksm@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/04/23 15:29:24 by jbrinksm       #+#    #+#                */
/*   Updated: 2019/04/23 15:34:05 by jbrinksm      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"

static int	return_test(char *line, int ret)
{
	if (line)
		ft_strdel(&line);
	return (ret);
}

static int	test_command_len_from_line_1(void)
{
	char	*line;
	char	*result;
	int		index;

	line = ft_strdup("simple;command;line");
	if (!line)
		return (return_test(NULL, NULL, E_ALLOC));
	index = 0;
	result = parser_strdup_command_from_line(line, &index);
	if (!result)
		return (return_test(line, NULL, E_ALLOC));
	if (ft_strcmp(result, "simple") != 0)
		return (return_test(line, result, FUNCT_FAILURE));
	return (return_test(line, result, FUNCT_SUCCESS));
}

static int	test_command_len_from_line_2(void)
{
	char	*line;
	char	*result;
	int		index;

	line = ft_strdup("more\\;complicated;line");
	if (!line)
		return (return_test(NULL, NULL, E_ALLOC));
	index = 0;
	result = parser_strdup_command_from_line(line, &index);
	if (!result)
		return (return_test(line, NULL, E_ALLOC));
	if (ft_strcmp(result, "more\\;complicated") != 0)
		return (return_test(line, result, FUNCT_FAILURE));
	return (return_test(line, result, FUNCT_SUCCESS));
}

static int	test_command_len_from_line_3(void)
{
	char	*line;
	char	*result;
	int		index;

	line = ft_strdup("\"quoted;command;line\"");
	if (!line)
		return (return_test(NULL, NULL, E_ALLOC));
	index = 0;
	result = parser_strdup_command_from_line(line, &index);
	if (!result)
		return (return_test(line, NULL, E_ALLOC));
	if (ft_strcmp(result, "quoted;command;line") != 0)
		return (return_test(line, result, FUNCT_FAILURE));
	return (return_test(line, result, FUNCT_SUCCESS));
}

int			test_command_len_from_line(void)
{
	int ret;

	ret = test_command_len_from_line_1();
	if (ret != FUNCT_SUCCESS)
		return (ret);
	ret = test_command_len_from_line_2();
	if (ret != FUNCT_SUCCESS)
		return (ret);
	ret = test_command_len_from_line_3();
	return (ret);
}
