/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parser_split_line_to_commands.c                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: jbrinksm <jbrinksm@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/04/23 14:03:51 by jbrinksm       #+#    #+#                */
/*   Updated: 2019/05/04 13:00:25 by jbrinksm      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

/*
**	NOTE: This code is nowhere near from perfect and any help and/or suggestions
**	are appreciated.
*/

#include "vsh.h"

void	parser_add_str_to_lst(char *arg, t_list **args)
{
	t_list	*probe;

	if (*args == NULL)
		*args = ft_lstnew(arg, ft_strlen(arg) + 1);
	else
	{
		probe = *args;
		while (probe->next != NULL)
			probe = probe->next;
		probe->next = ft_lstnew(arg, ft_strlen(arg) + 1);
	}
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
	while (line[i] != '\0' && is_uninhibited_semicolon(line, i, quote) != true)
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
		while (ft_isblank(line[i]) == true)
			i++;
		len = parser_strlen_cmd(&line[i]);
		if (len > 0)
		{
			command = ft_strndup(&line[i], len);
			parser_add_str_to_lst(command, &command_lst);
			ft_strdel(&command);
			i += len;
		}
		else
			i++;
	}
	return (command_lst);
}
