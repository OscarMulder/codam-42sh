/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parser_split_command_to_args.c                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: jbrinksm <marvin@codam.nl>                   +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/05/02 20:48:21 by jbrinksm       #+#    #+#                */
/*   Updated: 2019/05/02 20:54:45 by jbrinksm      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"

t_list	*parser_split_command_to_args(char *cmd)
{
	CMD_LIST	*args;
	char		*arg;
	int			i;
	int			len;

	args = NULL;
	i = 0;
	while (cmd[i] != '\0')
	{
		while (is_uninhibited_blank(cmd, i) == true)
			i++;
		len = 
	}
}