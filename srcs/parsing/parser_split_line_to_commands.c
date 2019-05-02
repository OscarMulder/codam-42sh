/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parser_split_line_to_commands.c                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: jbrinksm <jbrinksm@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/04/23 14:03:51 by jbrinksm       #+#    #+#                */
/*   Updated: 2019/05/02 17:35:06 by jbrinksm      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

/*
**	NOTE: This code is nowhere near from perfect and any help and/or suggestions
**	are appreciated.
*/

#include "vsh.h"

void		parser_add_command_to_lst(char *command, ARG_LIST **args)
{
	ARG_LIST	*probe;

	if (*args == NULL)
		*args = ft_lstnew(command, ft_strlen(command));
	else
	{
		probe = *args;
		while (probe->next != NULL)
			probe = probe->next;
		probe->next = ft_lstnew(command, ft_strlen(command));
	}
}

int			is_uninhibited_blank(char *command, int i)
{
	char	quote;

	quote = '\0';
	if (is_char_escaped(command, i) == FUNCT_SUCCESS)
		return (FUNCT_SUCCESS);
	if (update_quote_status(command, i, &quote) == FUNCT_SUCCESS)
		return (FUNCT_SUCCESS);
	return (FUNCT_FAILURE);
}

CMD_LIST	*parser_split_line_to_commands(char *line)
{
	t_list	*commands;
	char	*command;
	int		i;

	while (line[i] != '\0')
	{
		if (is_uninhibited_blank(line, i) == 1)
			i++;
		else
		{
			command = parser_
		}
	}
}
