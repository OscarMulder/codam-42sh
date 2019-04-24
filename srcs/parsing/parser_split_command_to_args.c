/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parser_split_command_to_args.c                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: jbrinksm <jbrinksm@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/04/24 15:47:28 by jbrinksm       #+#    #+#                */
/*   Updated: 2019/04/24 16:58:47 by jbrinksm      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

/*
**	This can probably be combined with parser_split_line_to_commands
**	into a general function.
*/

#include "vsh.h"

/*
**	Counts the total args (delimited by an unsescaped unquoted ' ' or '\t').
*/

int			parser_total_args_from_command(char *command)
{
	int		index;
	int		total;
	char	quote;

	index = 0;
	total = 1;
	quote = '\0';
	while (command[index] != '\0')
	{
		update_quote_status(command, index, &quote);
		if (quote == '\0' && (command[index] == ' ' || command[index] == '\t'))
		{
			if (is_char_escaped(command, index) == FUNCT_FAILURE)
				total++;
		}
		index++;
	}
	return (total);
}

int			parser_arg_len_from_command(char *command, int *start_arg)
{
	int		len;
	int		index;
	char	quote;

	len = 0;
	index = *start_arg;
	quote = '\0';
	while (command[index] != '\0')
	{
		if (update_quote_status(command, index, &quote) == FUNCT_SUCCESS)
			len--;
		if (quote == '\0' && (command[index] == ' ' || command[index] == '\t') \
		&& is_char_escaped(command, index) == FUNCT_FAILURE)
		{
			while (command[index] == ' ' || command[index] == '\t')
				index++;
			*start_arg = index;
			break ;
		}
		len++;
		index++;
	}
	return (len);
}

char		*parser_strdup_arg_from_command(char *command, int *start_arg)
{
	char	*arg;
	int		command_index;
	char	quote;
	int		len;
	int		arg_index;

	command_index = *start_arg;
	len = parser_arg_len_from_command(command, start_arg);
	arg = ft_strnew(len);
	if (arg == NULL)
		return (NULL);
	arg_index = 0;
	quote = '\0';
	while (arg_index < len)
	{
		if (update_quote_status(command, command_index, &quote) \
		== FUNCT_SUCCESS)
			command_index++;
		arg[arg_index] = command[command_index];
		command_index++;
		arg_index++;
	}
	return (arg);
}

char		**parser_split_command_to_args(char *command)
{
	char	**args;
	int		total;
	int		args_index;
	int		start_arg;

	total = parser_total_args_from_command(command);
	args = (char**)ft_memalloc(sizeof(char*) * (total + 1));
	if (args == NULL)
		return (NULL);
	args_index = 0;
	start_arg = 0;
	while (args_index < total)
	{
		args[args_index] = parser_strdup_arg_from_command(command, &start_arg);
		if (args[args_index] == NULL)
		{
			ft_freearray(&args);
			return (NULL);
		}
		args_index++;
	}
	args[args_index] = NULL;
	return (args);
}
