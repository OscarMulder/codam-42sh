/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parser_lexer.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: jbrinksm <jbrinksm@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/04/17 14:57:49 by jbrinksm       #+#    #+#                */
/*   Updated: 2019/04/22 18:57:09 by jbrinksm      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

/*
**	IMPORTANT: add check for more than one ';' command seperator.
**	It's not allowed.
*/

#include "vsh.h"

/*
**	Checks the given string with it's given index, and looks back to check if
**	the line[cur_index] is escaped.
**
**	Returns FUNCT_SUCCESS if escaped, FUNCT_FAILURE if not escaped.
*/

static int	is_char_escaped(char *line, int cur_index)
{
	int index;
	int	total_escape_chars;

	index = cur_index - 1;
	total_escape_chars = 0;
	if (index < 0)
		return (FUNCT_FAILURE);
	if (line[index] == '\\')
	{
		while (line[index] == '\\' && index >= 0)
		{
			total_escape_chars++;
			index--;
		}
		if (total_escape_chars % 2 == 1)
			return (FUNCT_SUCCESS);
		else
			return (FUNCT_FAILURE);
	}
	return (FUNCT_FAILURE);
}

/*
**	Updates a char variable pointed to by `char *quote` if the line[cur_index]
**	is a closing quote or opening quote.
**
**	Quote will be set to '\'' or '"' if it was a opening quote, and reset to
**	'\0' if it was a closing quote. If line[cur_index] isn't a quote, nothing
**	will happen.
**
**	Returns FUNCT_FAILURE if `char *quote` wasn't updated, FUNCT_SUCCESS if it
**	updated.
*/

static int	update_quote_status(char *line, int cur_index, char *quote)
{
	int		is_escaped;
	char	c;

	is_escaped = is_char_escaped(line, cur_index);
	c = line[cur_index];
	if (is_escaped == 0)
	{
		if (*quote == '\0' && (c == '"' || c == '\''))
			*quote = c;
		else if (c == *quote)
			*quote = '\0';
		else
			return (FUNCT_FAILURE);
	}
	else
		return (FUNCT_FAILURE);
	return (FUNCT_SUCCESS);
}

/*
**	Counts the total commands (delimited by a ';').
*/

static int	parser_total_commands(char *line)
{
	int		index;
	int		total;
	char	quote;

	index = 0;
	total = 1;
	quote = '\0';
	while (line[index] != '\0')
	{
		update_quote_status(line, index, &quote);
		if (quote == '\0' && line[index] == ';')
		{
			if (is_char_escaped(line, index) == FUNCT_FAILURE)
				total++;
		}
		index++;
	}
	return (total);
}

static int	parser_arg_len(char *line, int *start_arg_index)
{
	int		len;
	int		index;
	char	quote;

	len = 0;
	index = *start_arg_index;
	quote = '\0';
	while (line[index] != '\0')
	{
		if (update_quote_status(line, index, &quote) == FUNCT_SUCCESS)
			len--;
		if (quote == '\0' && line[index] == ';' && \
		is_char_escaped(line, index) == FUNCT_FAILURE)
		{
			*start_arg_index = index + 1;
			break ;
		}
		len++;
		index++;
	}
	return (len);
}

static char	*parser_fill_arg(char *line, int *start_arg_index)
{
	char	*arg;
	int		line_index;
	char	quote;
	int		len;
	int		index;

	line_index = *start_arg_index;
	len = parser_arg_len(line, start_arg_index);
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

static char	**parser_split_commands(char *line)
{
	char	**commands;
	int		total;
	int		arg_index;
	int		start_arg_index;

	total = parser_total_commands(line);
	commands = (char**)malloc(sizeof(char*) * total + 1);
	if (commands == NULL)
		return (NULL);
	arg_index = 0;
	start_arg_index = 0;
	while (arg_index < total)
	{
		commands[arg_index] = parser_fill_arg(line, &start_arg_index);
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

/*
**	The first part of the parser. This function will split line into
**	commands and set `char **commands`. Will deal with quoting. Commands are
**	delimited by a ';'. Will NOT do any expansions or wildcard management.
**
**	Returns FUNCT_SUCCESS if succesful, returns FUNCT_FAILURE if error occured.
*/

int			parser_lexer(char *line, char ***commands)
{
	*commands = parser_split_commands(line);
	if (*commands == NULL)
		return (FUNCT_FAILURE);
	return (FUNCT_SUCCESS);
}
