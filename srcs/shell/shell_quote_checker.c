/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   shell_quote_checker.c                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: jbrinksm <jbrinksm@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/05/31 07:47:19 by jbrinksm       #+#    #+#                */
/*   Updated: 2019/06/02 08:52:11 by jbrinksm      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"

char		shell_quote_checker_find_quote(char *line)
{
	int		i;
	char	quote;
	char	c;

	i = 0;
	quote = '\0';
	while (line[i] != '\0')
	{
		if (tools_is_char_escaped(line, i) == true)
		{
			i++;
			continue ;
		}
		c = line[i];
		if (quote == '\0' && (c == '\'' || c == '"'))
			quote = c;
		else if (quote && c == quote)
			quote = '\0';
		i++;
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
