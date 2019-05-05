/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parser_lexer.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: jbrinksm <jbrinksm@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/04/17 14:57:49 by jbrinksm       #+#    #+#                */
/*   Updated: 2019/05/05 12:26:52 by jbrinksm      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

/*
**	IMPORTANT: add check for more than one subsequent ';' command seperator.
**	It's not allowed.
*/

#include "vsh.h"

/*
**	Function to get the result of the split_command_to_args into the cmd_tab
**	list.
**
**	Make sure that the cmd_tab pointer is initialized to NULL if it doesnt exist
**	yet.
*/

void	parser_add_lst_to_lst(t_list *args, t_list **cmd_tab)
{
	t_list	*probe;

	if (*cmd_tab == NULL)
	{
		*cmd_tab = ft_lstnew(NULL, 0);
		(*cmd_tab)->content = args;
	}
	else
	{
		probe = *cmd_tab;
		while (probe->next != NULL)
			probe = probe->next;
		probe->next = ft_lstnew(NULL, 0);
		probe->next->content = args;
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
**	The resulting cmd_tab is structured as follows:
**
**	t_list* cmd_tab----> content(t_list* args)----------> content(char* of arg1)
**			|								|
**			|							next|
**			|		(1st command)		item|-----------> content(char* of arg2)
**			|								|
**			|							next|
**			|							item|-----------> content(char* of arg3)
**			|								|
**			|							next|
**			|							item= NULL
**		next|
**		item|----------> content(t_list* args)----------> content(char* of arg1)
**			|								|
**			|							next|
**			|		(2nd command)		item|-----------> content(char* of arg2)
**			|								|
**			|							next|
**			|							item|-----------> content(char* of arg3)
**			|								|
**			|							next|
**			|							item= NULL
**		next|
**		item|----------> content(t_list* args)----------> content(char* of arg1)
**			|								|
**			|							next|
**			|		(3rd command)		item|-----------> content(char* of arg2)
**			|								|
**		   ...							   ...
*/

int		parser_lexer(char *line, t_list **cmd_tab)
{
	t_list		*cmdstr_lst;
	t_list		*probe;
	t_list		*args;

	cmdstr_lst = parser_split_line_to_commands(line);
	parser_rem_esc_char_semicolons(cmdstr_lst);
	probe = cmdstr_lst;
	while (probe != NULL)
	{
		if (probe->content != NULL)
		{
			args = parser_split_command_to_args(probe->content);
			parser_remove_quotes(args);
			parser_rem_esc_char_quotes(args);
			parser_rem_esc_char_blanks(args);
			parser_add_lst_to_lst(args, cmd_tab);
		}
		probe = probe->next;
	}
	return (FUNCT_SUCCESS);
}
