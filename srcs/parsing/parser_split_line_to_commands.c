/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parser_split_line_to_commands.c                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: jbrinksm <jbrinksm@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/04/23 14:03:51 by jbrinksm       #+#    #+#                */
/*   Updated: 2019/04/29 16:42:03 by jbrinksm      ########   odam.nl         */
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
	int		i;
	int		total;
	char	quote;

	i = 0;
	total = 1;
	quote = '\0';
	while (line[i] != '\0')
	{
		update_quote_status(line, i, &quote);
		if (quote == '\0' && line[i] == ';')
		{
			if (is_char_escaped(line, i) == FUNCT_FAILURE)
				total++;
		}
		i++;
	}
	return (total);
}

int			parser_command_len_from_line(char *line, int *start_arg_index)
{
	int		len;
	int		i;
	char	quote;

	len = 0;
	i = *start_arg_index;
	quote = '\0';
	while (line[i] != '\0')
	{
		if (update_quote_status(line, i, &quote) == FUNCT_SUCCESS)
			len--;
		if (quote == '\0' && line[i] == ';' && \
		is_char_escaped(line, i) == FUNCT_FAILURE)
		{
			*start_arg_index = i + 1;
			break ;
		}
		len++;
		i++;
	}
	return (len);
}

/*
**	Extracts a command from line
*/

char		*parser_strdup_command_from_line(char *line, int *start_arg_index)
{
	char	*arg;
	int		line_index;
	char	quote;
	int		len;
	int		index;

	line_index = *start_arg_index;
	len = parser_command_len_from_line(line, start_arg_index);
	arg = ft_strnew(len);
	if (arg == NULL)
		return (NULL);
	index = 0;
	quote = '\0';
	while (index < len)
	{
		if (update_quote_status(line, line_index, &quote) == FUNCT_SUCCESS)
			line_index++;
		arg[index] = line[line_index];
		line_index++;
		index++;
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
	int		arg_index;
	int		start_arg_index;

	total = parser_total_commands_from_line(line);
	commands = (char**)ft_memalloc(sizeof(char*) * (total + 1));
	if (commands == NULL)
		return (NULL);
	arg_index = 0;
	start_arg_index = 0;
	while (arg_index < total)
	{
		commands[arg_index] = \
		parser_strdup_command_from_line(line, &start_arg_index);
		if (commands[arg_index] == NULL)
		{
			ft_freearray(&commands);
			return (NULL);
		}
		arg_index++;
	}
	commands[arg_index] = NULL;
	return (commands);
}
