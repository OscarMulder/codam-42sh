/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parser_lexer.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: jbrinksm <jbrinksm@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/04/17 14:57:49 by jbrinksm       #+#    #+#                */
/*   Updated: 2019/05/02 19:42:33 by jbrinksm      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

/*
**	IMPORTANT: add check for more than one subsequent ';' command seperator.
**	It's not allowed.
*/

#include "vsh.h"

int			parser_lexer(char *line, CMD_LIST **cmd_tab)
{
	t_list		*commands;

	(void)cmd_tab;
	commands = parser_split_line_to_commands(line);
	int i = 0;
	ft_putendl("");
	while (commands != NULL)
	{
		if (commands->content != NULL)
			ft_printf("cmd [%i]>%s<\n", i, commands->content);
		commands = commands->next;
		i++;
	}
	return (FUNCT_SUCCESS);
}
