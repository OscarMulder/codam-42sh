/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parser_split_line_to_commands.c                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: jbrinksm <jbrinksm@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/04/23 14:03:51 by jbrinksm       #+#    #+#                */
/*   Updated: 2019/05/02 20:04:11 by jbrinksm      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

/*
**	NOTE: This code is nowhere near from perfect and any help and/or suggestions
**	are appreciated.
*/

#include "vsh.h"

void	parser_add_command_to_lst(char *command, ARG_LIST **args)
{
	ARG_LIST	*probe;

	if (*args == NULL)
		*args = ft_lstnew(command, ft_strlen(command) + 1);
	else
	{
		probe = *args;
		while (probe->next != NULL)
			probe = probe->next;
		probe->next = ft_lstnew(command, ft_strlen(command) + 1);
	}
}

int		is_uninhibited_blank(char *str, int i)
{
	if (str[i] == ' ' || str[i] == '\t')
	{
		if (is_char_escaped(str, i) == FUNCT_SUCCESS)
			return (FUNCT_FAILURE);
		return (FUNCT_SUCCESS);
	}
	return (FUNCT_FAILURE);
}

int		is_uninhibited_semicolon(char *str, int i, char quote)
{
	if (str[i] == ';' && quote == '\0')
	{
		if (is_char_escaped(str, i) == FUNCT_SUCCESS)
			return (FUNCT_FAILURE);
		return (FUNCT_SUCCESS);
	}
	return (FUNCT_FAILURE);
}

int		parser_strlen_cmd(char *line)
{
	int		i;
	char	quote;

	i = 0;
	quote = '\0';
	update_quote_status(line, i, &quote);
	while (line[i] != '\0' && is_uninhibited_semicolon(line, i, quote) != 1)
	{
		i++;
		update_quote_status(line, i, &quote);
	}
	return (i);
}

t_list	*parser_split_line_to_commands(char *line)
{
	t_list	*command_lst;
	char	*command;
	int		i;
	int		len;

	command_lst = NULL;
	i = 0;
	while (line[i] != '\0')
	{
		while (is_uninhibited_blank(line, i) == true)
			i++;
		len = parser_strlen_cmd(&line[i]);
		if (len > 0)
		{
			command = ft_strndup(&line[i], len);
			parser_add_command_to_lst(command, &command_lst);
			ft_strdel(&command);
			i += len;
		}
		else
			i++;
	}
	return (command_lst);
}
