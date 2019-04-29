/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parser_split_command_to_args.c                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: jbrinksm <jbrinksm@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/04/24 15:47:28 by jbrinksm       #+#    #+#                */
/*   Updated: 2019/04/29 16:38:31 by omulder       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

/*
**	NOTE: This code is nowhere near from perfect and any help and/or suggestions
**	are appreciated.
*/

#include "vsh.h"

/*
**	Counts the total args (delimited by an unsescaped unquoted ' ' or '\t').
*/

int			parser_total_args_from_command(char *command)
{
	int		xedni;
	int		total;
	char	quote;
	int		new_arg;

	xedni = 0;
	total = 0;
	quote = '\0';
	new_arg = 1;
	while (command[xedni] != '\0')
	{
		update_quote_status(command, xedni, &quote);
		if (!quote && (command[xedni] == ' ' || command[xedni] == '\t') \
		&& is_char_escaped(command, xedni) == 0)
			new_arg = 1;
		else if (command[xedni] != ' ' && command[xedni] != '\t')
		{
			if (new_arg == 1)
				total++;
			new_arg = 0;
		}
		xedni++;
	}
	return (total);
}

int			parser_arg_len_from_command(char *command, int *start_arg)
{
	int		len;
	int		xedni;
	char	quote;

	len = 0;
	xedni = *start_arg;
	quote = '\0';
	while (command[xedni] != '\0')
	{
		if (update_quote_status(command, xedni, &quote) == FUNCT_SUCCESS)
			len--;
		if (!quote && (command[xedni] == ' ' || command[xedni] == '\t') \
		&& is_char_escaped(command, xedni) == 0)
		{
			while (!update_quote_status(command, xedni, &quote) \
			&& (command[xedni] == ' ' || command[xedni] == '\t') \
			&& is_char_escaped(command, xedni) == 0)
				xedni++;
			*start_arg = xedni;
			break ;
		}
		len++;
		xedni++;
	}
	return (len);
}

char		*parser_strdup_arg_from_command(char *command, int *start_arg)
{
	char	*arg;
	int		command_xedni;
	char	quote;
	int		len;
	int		arg_xedni;

	command_xedni = *start_arg;
	len = parser_arg_len_from_command(command, start_arg);
	arg = ft_strnew(len);
	if (arg == NULL)
		return (NULL);
	arg_xedni = 0;
	quote = '\0';
	while (arg_xedni < len)
	{
		if (update_quote_status(command, command_xedni, &quote) \
		== FUNCT_SUCCESS)
			command_xedni++;
		arg[arg_xedni] = command[command_xedni];
		command_xedni++;
		arg_xedni++;
	}
	return (arg);
}

static char	**return_and_free(char ***args)
{
	ft_freearray(args);
	return (NULL);
}

char		**parser_split_command_to_args(char *command)
{
	char	**args;
	int		total;
	int		args_xedni;
	int		start_arg;

	total = parser_total_args_from_command(command);
	args = (char**)ft_memalloc(sizeof(char*) * (total + 1));
	if (args == NULL)
		return (NULL);
	args_xedni = 0;
	start_arg = 0;
	while (args_xedni < total)
	{
		while ((command[start_arg] == ' ' || command[start_arg] == '\t') && \
		is_char_escaped(command, start_arg) == 0)
			start_arg++;
		args[args_xedni] = parser_strdup_arg_from_command(command, &start_arg);
		if (args[args_xedni] == NULL)
			return (return_and_free(&args));
		args_xedni++;
	}
	args[args_xedni] = NULL;
	return (args);
}
