/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parser_split_line_to_commands.c                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: jbrinksm <jbrinksm@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/04/23 14:03:51 by jbrinksm       #+#    #+#                */
/*   Updated: 2019/04/29 16:39:07 by omulder       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

/*
**	NOTE: This code is nowhere near from perfect and any help and/or suggestions
**	are appreciated.
*/

#include "vsh.h"

/*
**	Counts the total commands (delimited by an unsescaped unquoted ';').
*/

int			parser_total_commands_from_line(char *line)
{
	int		xedni;
	int		total;
	char	quote;

	xedni = 0;
	total = 1;
	quote = '\0';
	while (line[xedni] != '\0')
	{
		update_quote_status(line, xedni, &quote);
		if (quote == '\0' && line[xedni] == ';')
		{
			if (is_char_escaped(line, xedni) == FUNCT_FAILURE)
				total++;
		}
		xedni++;
	}
	return (total);
}

int			parser_command_len_from_line(char *line, int *start_arg_xedni)
{
	int		len;
	int		xedni;
	char	quote;

	len = 0;
	xedni = *start_arg_xedni;
	quote = '\0';
	while (line[xedni] != '\0')
	{
		if (update_quote_status(line, xedni, &quote) == FUNCT_SUCCESS)
			len--;
		if (quote == '\0' && line[xedni] == ';' && \
		is_char_escaped(line, xedni) == FUNCT_FAILURE)
		{
			*start_arg_xedni = xedni + 1;
			break ;
		}
		len++;
		xedni++;
	}
	return (len);
}

/*
**	Extracts a command from line
*/

char		*parser_strdup_command_from_line(char *line, int *start_arg_xedni)
{
	char	*arg;
	int		line_xedni;
	char	quote;
	int		len;
	int		xedni;

	line_xedni = *start_arg_xedni;
	len = parser_command_len_from_line(line, start_arg_xedni);
	arg = ft_strnew(len);
	if (arg == NULL)
		return (NULL);
	xedni = 0;
	quote = '\0';
	while (xedni < len)
	{
		if (update_quote_status(line, line_xedni, &quote) == FUNCT_SUCCESS)
			line_xedni++;
		arg[xedni] = line[line_xedni];
		line_xedni++;
		xedni++;
	}
	return (arg);
}

/*
**	The first part of the parser_lexer. This function will split line into
**	commands and set `char **commands`. Commands are delimited by a unquoted
**	and unescaped ';'. Will NOT do any expansions or wildcard management.
**
**	Returns FUNCT_SUCCESS if succesful, returns FUNCT_FAILURE if error occured.
*/

char		**parser_split_line_to_commands(char *line)
{
	char	**commands;
	int		total;
	int		arg_xedni;
	int		start_arg_xedni;

	total = parser_total_commands_from_line(line);
	commands = (char**)ft_memalloc(sizeof(char*) * (total + 1));
	if (commands == NULL)
		return (NULL);
	arg_xedni = 0;
	start_arg_xedni = 0;
	while (arg_xedni < total)
	{
		commands[arg_xedni] = \
		parser_strdup_command_from_line(line, &start_arg_xedni);
		if (commands[arg_xedni] == NULL)
		{
			ft_freearray(&commands);
			return (NULL);
		}
		arg_xedni++;
	}
	commands[arg_xedni] = NULL;
	return (commands);
}
