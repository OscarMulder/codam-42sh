/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   shell_quote_checker.c                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: jbrinksm <jbrinksm@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/05/31 07:47:19 by jbrinksm       #+#    #+#                */
/*   Updated: 2019/05/31 07:48:22 by root          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"

char		shell_quote_checker_find_quote(char *line)
{
	int		index;
	char	quote;
	int		is_escaped;
	char	c;

	index = 0;
	quote = '\0';
	while (line[index] != '\0')
	{
		is_escaped = tools_is_char_escaped(line, index);
		c = line[index];
		if (!quote && (c == '\'' || c == '"') && !is_escaped)
			quote = c;
		else if (quote && c == quote && !is_escaped)
			quote = '\0';
		index++;
	}
	return (quote);
}

void		shell_quote_checker(char **line)
{
	char	quote;
	char	*extra_line;

	quote = shell_quote_checker_find_quote(*line);
	while (quote != '\0')
	{
		if (quote == '\'')
			ft_printf("\nquote> ");
		else if (quote == '"')
			ft_printf("\ndquote> ");
		input_read(&extra_line);
		*line = ft_joinstrcstr_free_all(*line, '\n', extra_line);
		quote = shell_quote_checker_find_quote(*line);
	}
}
