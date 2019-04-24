/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parser_lexer.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: jbrinksm <jbrinksm@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/04/17 14:57:49 by jbrinksm       #+#    #+#                */
/*   Updated: 2019/04/24 17:44:03 by jbrinksm      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

/*
**	IMPORTANT: add check for more than one ';' command seperator.
**	It's not allowed.
*/

#include "vsh.h"

int			parser_lexer(char *line, char ****cmd_tab)
{
	char	**commands;
	int		index;
	int		len;

	commands = parser_split_line_to_commands(line);
	if (commands == NULL)
		return (FUNCT_FAILURE);
	len = ft_arraylen(commands);
	*cmd_tab = (char***)ft_memalloc(sizeof(char**) * \
	(len + 1));
	index = 0;
	while (index < len)
	{
		(*cmd_tab)[index] = parser_split_command_to_args(commands[index]);
		if ((*cmd_tab)[index] == NULL)
		{
			ft_freearray(&commands);
		}
		index++;
	}
	(*cmd_tab)[index] = NULL;
	return (FUNCT_SUCCESS);
}
