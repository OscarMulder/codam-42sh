/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parser_split_command_to_args.c                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: jbrinksm <jbrinksm@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/04/24 15:47:28 by jbrinksm       #+#    #+#                */
/*   Updated: 2019/04/29 20:34:27 by jbrinksm      ########   odam.nl         */
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

int		parser_arg_len_from_command(char *command)
{
	int		len;
	int		i;
	char	quote;

	len = 0;
	i = 0;
	quote = '\0';
	while (command[i] != '\0')
	{
		update_quote_status(command, i, &quote);
		if (!quote && ft_isblank(command[i]) \
		&& is_char_escaped(command, i) == 0)
			break ;
		len++;
		i++;
	}
	return (len);
}

char	*parser_strdup_arg_from_command(char *command)
{
	char	*arg;
	int		command_index;
	char	quote;
	int		len;
	int		arg_index;

	command_index = 0;
	len = parser_arg_len_from_command(command);
	arg = ft_strnew(len);
	if (arg == NULL)
		return (NULL);
	arg_index = 0;
	quote = '\0';
	while (arg_index < len)
	{
		arg[arg_index] = command[command_index];
		command_index++;
		arg_index++;
	}
	return (arg);
}

int			is_char_inhibited(char *command, int i)
{
	char	quote;

	quote = '\0';
	if (is_char_escaped(command, i) == FUNCT_SUCCESS)
		return (FUNCT_SUCCESS);
	if (update_quote_status(command, i, &quote) == FUNCT_SUCCESS)
		return (FUNCT_SUCCESS);
	return (FUNCT_FAILURE);
}

void		add_arg(char *command, ARG_LIST **args)
{
	char		*arg;
	ARG_LIST	*probe;

	arg = parser_strdup_arg_from_command(command);
	if (arg == NULL)
		return ;
	if (*args == NULL)
	{
		*args = ft_lstnew(arg, ft_strlen(arg + 1));
		if (*args == NULL)
			free(arg);
	}
	else
	{
		probe = *args;
		while (probe->next != NULL)
			probe = probe->next;
		probe->next = ft_lstnew(arg, ft_strlen(arg + 1));
		if (probe->next == NULL)
			free(arg);
	}
}

ARG_LIST	*parser_split_command_to_args(char *command)
{
	int			i;
	ARG_LIST	*args;
	int			len;

	i = 0;
	args = NULL;
	while (command[i] != '\0')
	{
		while (is_char_inhibited(command, i) == 0 && ft_isblank(command[i]))
			i++;
		len = parser_arg_len_from_command(&command[i]);
		add_arg(&command[i], &args);
		i += len;
	}
	return (args);
}

/*
**	Will have to rewrite parser_lexer where it doesn't remove quotes because of
**	index issues. Otherwise it will be messy, and this way we can stay
**	consistent when removing escape chars.
*/
