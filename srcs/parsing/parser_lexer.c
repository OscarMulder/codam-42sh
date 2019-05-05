/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parser_lexer.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: jbrinksm <jbrinksm@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/04/17 14:57:49 by jbrinksm       #+#    #+#                */
/*   Updated: 2019/05/05 12:06:32 by jbrinksm      ########   odam.nl         */
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

/*
**	The goal of this function is to simple seperate the input line into
**	a command table (cmd_tab). Firstly the commands are seperated by ';' chars,
**	then the command lines themselves are seperated by '\t' and ' ' chars.
**	Naturally, we are keeping track of quotes and escape chars along the way.
**	The uninhibited(!) ';', '\'', '"', \t', and ' ' chars are removed, as are
**	the escape chars that escaped one of those chars, since they no longer
**	serve a special meaning in the parser after this.
**	The resulting CMD_LIST command table is structured as follows:
**
**	CMD_LIST(cmd_tab)--> content(ARG_LST of command 1) -> content(char* of arg1)
**			|								|
**			|							next|
**			|		(1st lst_item)		item|-----------> content(char* of arg2)
**			|								|
**			|							next|
**			|							item|-----------> content(char* of arg3)
**			|								|
**			|							next|
**			|							item= NULL
**		next|
**		item|----------> content(ARG_LST of command 2) -> content(char* of arg1)
**			|								|
**			|							next|
**			|		(2nd lst_item)		item|-----------> content(char* of arg2)
**			|								|
**			|							next|
**			|							item|-----------> content(char* of arg3)
**			|								|
**			|							next|
**			|							item= NULL
**		next|
**		item|----------> content(ARG_LST of command 3) -> content(char* of arg1)
**			|								|
**			|							next|
**			|		(3rd lst_item)		item|-----------> content(char* of arg2)
**			|								|
**		   ...							   ...
*/

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
