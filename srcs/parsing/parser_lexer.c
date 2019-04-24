/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parser_lexer.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: jbrinksm <jbrinksm@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/04/17 14:57:49 by jbrinksm       #+#    #+#                */
/*   Updated: 2019/04/24 18:50:24 by jbrinksm      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

/*
**	IMPORTANT: add check for more than one ';' command seperator.
**	It's not allowed.
*/

#include "vsh.h"

static void	freetable(char ****table)
{
	int index;

	if (*table != NULL)
	{
		index = 0;
		while ((*table)[index] != NULL)
		{
			ft_freearray(&((*table)[index]));
			index++;
		}
		free(*table);
		*table = NULL;
	}
}

static int	return_free(char ****cmd_tab, char **commands, int ret)
{
	if (ret != FUNCT_SUCCESS)
		freetable(cmd_tab);
	ft_freearray(&commands);
	return (ret);
}

int			parser_lexer(char *line, char ****cmd_tab)
{
	char	**commands;
	int		index;
	int		len;

	commands = parser_split_line_to_commands(line);
	if (commands == NULL)
		return (FUNCT_FAILURE);
	len = ft_arraylen(commands);
	*cmd_tab = (char***)ft_memalloc(sizeof(char**) * (len + 1));
	if (*cmd_tab == NULL)
		return (return_free(cmd_tab, commands, FUNCT_FAILURE));
	index = 0;
	while (index < len)
	{
		(*cmd_tab)[index] = parser_split_command_to_args(commands[index]);
		if ((*cmd_tab)[index] == NULL)
			return (return_free(cmd_tab, commands, FUNCT_FAILURE));
		index++;
	}
	(*cmd_tab)[index] = NULL;
	return (return_free(cmd_tab, commands, FUNCT_SUCCESS));
}
