/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   lexer_debug.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: mavan-he <mavan-he@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/05/16 16:06:49 by mavan-he       #+#    #+#                */
/*   Updated: 2019/09/16 12:38:05 by mavan-he      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"
#define DEBUG
#ifdef DEBUG


void	print_node(t_tokenlst *node)
{
	ft_printf("%-16s", parser_return_token_str(node->type));
	if (node->type == IO_NUMBER)
		ft_printf("%-16s", node->value);
	else if (node->type == WORD || node->type == ASSIGN)
		ft_printf("%-16s", node->value);
	else
		ft_printf("%-16s", parser_return_token_str(node->type));
	if (node->flags & T_FLAG_HASSPECIAL)
		ft_putstr("<has_special>");
	ft_putchar('\n');
}

void	get_type(t_scanner *scanner, char **type)
{
	if (scanner->tk_type == WORD)
		*type = "WORD";
	else if (scanner->tk_type == PIPE)
		*type = "PIPE";
	else if (scanner->tk_type == SLESS)
		*type = "SLESS";
	else if (scanner->tk_type == SGREAT)
		*type = "SGREAT";
	else if (scanner->tk_type == DLESS)
		*type = "DLESS";
	else if (scanner->tk_type == DGREAT)
		*type = "DGREAT";
	else if (scanner->tk_type == BG)
		*type = "BG";
	else if (scanner->tk_type == SEMICOL)
		*type = "SEMICOLON";
	else if (scanner->tk_type == AND_IF)
		*type = "AND_IF";
	else if (scanner->tk_type == OR_IF)
		*type = "OR_IF";
	else if (scanner->tk_type == NEWLINE)
		*type = "NEWLINE";
}

void	print_token(t_scanner *scanner)
{
	char	*type;
	char	*lexeme;

	get_type(scanner, &type);
	ft_printf("\n%s (%i): ", type, scanner->tk_len);
	lexeme = ft_strndup(&(scanner->str[scanner->str_index - scanner->tk_len]),
		scanner->tk_len);
	ft_printf("\t%s\n", lexeme);
	ft_strdel(&lexeme);
}

void	print_token_list(t_tokenlst *node)
{
	if (node == NULL)
		return ;
	print_node(node);
	print_token_list(node->next);
}
#endif
