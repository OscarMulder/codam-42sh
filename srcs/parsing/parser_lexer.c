/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parser_lexer.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: jbrinksm <jbrinksm@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/04/17 14:57:49 by jbrinksm       #+#    #+#                */
/*   Updated: 2019/05/04 21:53:58 by jbrinksm      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

/*
**	IMPORTANT: add check for more than one subsequent ';' command seperator.
**	It's not allowed.
*/

#include "vsh.h"

/*
**	Be sure that the t_list* is initialized to NULL if it doesnt exist yet.
*/

void	parser_add_lst_to_lst(t_list *content_lst, t_list **lst)
{
	t_list	*probe;

	if (*lst == NULL)
	{
		*lst = ft_lstnew(NULL, 0);
		(*lst)->content = content_lst;
	}
	else
	{
		probe = *lst;
		while (probe->next != NULL)
			probe = probe->next;
		probe->next = ft_lstnew(NULL, 0);
		probe->next->content = content_lst;
	}
}

int		parser_lexer(char *line, CMD_LIST **cmd_tab)
{
	t_list		*cmdstr_lst;
	t_list		*probe;
	t_list		*content;

	cmdstr_lst = parser_split_line_to_commands(line);
	parser_rem_esc_char_semicolons(cmdstr_lst);
	probe = cmdstr_lst;
	while (probe != NULL)
	{
		if (probe->content != NULL)
		{
			content = parser_split_command_to_args(probe->content);
			parser_remove_quotes(content);
			parser_rem_esc_char_quotes(content);
			parser_rem_esc_char_blanks(content);
			parser_add_lst_to_lst(content, cmd_tab);
		}
		probe = probe->next;
	}
	return (FUNCT_SUCCESS);
}
