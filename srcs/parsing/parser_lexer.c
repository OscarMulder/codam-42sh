/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parser_lexer.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: jbrinksm <jbrinksm@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/04/17 14:57:49 by jbrinksm       #+#    #+#                */
/*   Updated: 2019/05/02 20:43:02 by jbrinksm      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

/*
**	IMPORTANT: add check for more than one subsequent ';' command seperator.
**	It's not allowed.
*/

#include "vsh.h"

int		parser_lexer(char *line, CMD_LIST **cmd_tab)
{
	t_list		*cmdstr_lst;

	(void)cmd_tab;
	cmdstr_lst = parser_split_line_to_commands(line);
	parser_remove_quotes(cmdstr_lst);
	parser_rem_esc_char_semicolons(cmdstr_lst);
	parser_rem_esc_char_quotes(cmdstr_lst);

	int i = 0;
	ft_putendl("");
	while (cmdstr_lst != NULL)
	{
		if (cmdstr_lst->content != NULL)
			ft_printf("cmd [%i]>%s<\n", i, cmdstr_lst->content);
		cmdstr_lst = cmdstr_lst->next;
		i++;
	}
	return (FUNCT_SUCCESS);
}
