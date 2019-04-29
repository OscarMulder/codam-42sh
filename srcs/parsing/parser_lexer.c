/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parser_lexer.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: jbrinksm <jbrinksm@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/04/17 14:57:49 by jbrinksm       #+#    #+#                */
/*   Updated: 2019/04/29 17:07:26 by jbrinksm      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

/*
**	IMPORTANT: add check for more than one ';' command seperator.
**	It's not allowed.
*/

#include "vsh.h"

CMD_LIST	*init_cmdtab(void)
{
	CMD_LIST	*head;

	head = (CMD_LIST*)ft_lstnew(ft_lstnew(NULL, 0), sizeof(ARG_LIST));
	if (head == NULL)
		return (NULL);
	if (head->content == NULL)
	{
		free(head);
		return (NULL);
	}
	return (head);
}

int			parser_lexer(char *line, CMD_LIST **cmd_tab)
{
	char	**commands;

	commands = parser_split_line_to_commands(line);
	ft_putendl("");
	ft_putstrarri(commands);
	*cmd_tab = init_cmdtab();
	return (FUNCT_SUCCESS);
}
