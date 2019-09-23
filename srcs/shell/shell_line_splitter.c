/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   shell_line_splitter.c                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: mavan-he <mavan-he@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/09/19 17:44:26 by mavan-he       #+#    #+#                */
/*   Updated: 2019/09/22 17:08:28 by mavan-he      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"

static void	update_quote_status(char c, char *quote)
{
	if (*quote == '\0')
		*quote = c;
	else if (c == *quote)
		*quote = '\0';
}

int		shell_line_count(char *line)
{
	char	quote;
	int		i;
	int		line_count;

	quote = '\0';
	i = 0;
	line_count = 0;
	while (line[i] != '\0')
	{
		if (line[i] == '\\' && quote != '\'' && line[i + 1] != '\0')
			i++;
		else if (line[i] == '\'' || line[i] == '\"')
			update_quote_status(line[i], &quote);
		else if (line[i] == '\n' && quote == '\0')
			line_count++;
		i++;
	}
	if (i > 0 && line[i -1] != '\n')
		line_count++;
	return (line_count);
}

char	**shell_line_splitter(t_vshdata *data)
{
	char	**lines;
	int		i;
	int		line_count;

	line_count = shell_line_count(data->line->line);
	lines = ft_strsplit(data->line->line, '\n');
	i = 0;
	while (lines[i])
	{
		lines[i] = ft_strjoin(lines[i], "\n");
		i++;
	}
	return (lines);
}
