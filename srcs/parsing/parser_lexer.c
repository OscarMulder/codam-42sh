/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parser_lexer.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: jbrinksm <jbrinksm@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/04/17 14:57:49 by jbrinksm       #+#    #+#                */
/*   Updated: 2019/05/02 17:20:21 by jbrinksm      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

/*
**	IMPORTANT: add check for more than one subsequent ';' command seperator.
**	It's not allowed.
*/

#include "vsh.h"

void		add_command(char *command, CMD_LIST **cmd_tab)
{
	ARG_LIST	*args;
	CMD_LIST	*probe;

	args = parser_split_command_to_args(command);
	if (args == NULL)
		return ;
	if (*cmd_tab == NULL)
	{
		*cmd_tab = ft_lstnew(args, sizeof(ARG_LIST*));
		if (*cmd_tab == NULL)
			free(args);
	}
	else
	{
		probe = *cmd_tab;
		while (probe->next != NULL)
			probe = probe->next;
		probe->next = ft_lstnew(args, sizeof(ARG_LIST*));
		if (probe->next == NULL)
			free(args);
	}
}

int			parser_lexer(char *line, CMD_LIST **cmd_tab)
{
	t_list		*commands;

	commands = parser_split_line_to_commands(line);
	return (FUNCT_SUCCESS);
}
