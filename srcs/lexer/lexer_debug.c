/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   lexer_debug.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: mavan-he <mavan-he@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/05/16 16:06:49 by mavan-he       #+#    #+#                */
/*   Updated: 2019/05/25 17:05:57 by mavan-he      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"

#ifdef DEBUG

char	*put_token(t_tokens type)
{
	char	*token;

	token = NULL;
	if (type == START)
		token = "START";
	else if (type == ASSIGN)
		token = "ASSIGN";
	else if (type == WORD)
		token = "WORD";
	else if (type == IO_NUMBER)
		token = "IO_NUM";
	else if (type == AND_IF)
		token = "AND_IF";
	else if (type == OR_IF)
		token = "OR_IF";
	else if (type == DLESS)
		token = "DLESS";
	else if (type == DGREAT)
		token = "DGREAT";
	else if (type == SLESS)
		token = "SLESS";
	else if (type == SGREAT)
		token = "SGREAT";
	else if (type == LESSAND)
		token = "LESSAND";
	else if (type == GREATAND)
		token = "GREATAND";
	else if (type == BG)
		token = "BG";
	else if (type == PIPE)
		token = "PIPE";
	else if (type == SEMICOL)
		token = "SEMICOL";
	else if (type == END)
		token = "END";
	else
		token = "ERROR";
	return (token);
}

void	print_node(t_tokenlst *node)
{
	ft_printf("%-16s", put_token(node->type));
	if (node->type == IO_NUMBER)
		ft_printf("%-16s", node->value);
	else if (node->type == WORD || node->type == ASSIGN)
		ft_printf("%-16s", node->value);
	else
		put_token(node->type);
	if (node->flags & T_FLAG_HASDOLLAR)
		ft_putstr("<has_dollar>");
	if (node->type != END)
		ft_putchar('\n');
}

void	print_token(t_scanner *scanner)
{
	char			*type;
	char	*lexeme;

	if (scanner->tk_type == WORD)
		type = "WORD";
	else if (scanner->tk_type == PIPE)
		type = "PIPE";
	else if (scanner->tk_type == SLESS)
		type = "SLESS";
	else if (scanner->tk_type == SGREAT)
		type = "SGREAT";
	else if (scanner->tk_type == DLESS)
		type = "DLESS";
	else if (scanner->tk_type == DGREAT)
		type = "DGREAT";
	else if (scanner->tk_type == BG)
		type = "BG";
	else if (scanner->tk_type == SEMICOL)
		type = "SEMICOLON";
	else if (scanner->tk_type == AND_IF)
		type = "AND_IF";
	else if (scanner->tk_type == OR_IF)
		type = "OR_IF";
	else if (scanner->tk_type == NEWLINE)
		type = "NEWLINE";
	else
		type = "UNKNOWN";
	ft_printf("\n%s (%i): ", type, scanner->tk_len);
	lexeme = ft_strndup(&(scanner->str[scanner->str_index - scanner->tk_len]), scanner->tk_len);
	ft_printf("\t%s\n", lexeme);
	ft_strdel(&lexeme);
}

#endif
