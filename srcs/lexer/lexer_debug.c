/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   lexer_debug.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: mavan-he <mavan-he@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/05/16 16:06:49 by mavan-he       #+#    #+#                */
/*   Updated: 2019/05/16 19:19:01 by mavan-he      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"

#ifdef DEBUG

void	put_token(t_token *node)
{
	if (node->type == START)
		ft_putstr("START");
	else if (node->type == ASSIGN)
		ft_putstr("ASSIGN");
	else if (node->type == WORD)
		ft_putstr("WORD");
	else if (node->type == EXPAND)
		ft_putstr("EXPAND");
	else if (node->type == NEWLINE)
		ft_putstr("NEWLINE");
	else if (node->type == IO_NUM)
		ft_putstr("IO_NUM");
	else if (node->type == AND_IF)
		ft_putstr("AND_IF");
	else if (node->type == OR_IF)
		ft_putstr("OR_IF");
	else if (node->type == DLESS)
		ft_putstr("DLESS");
	else if (node->type == DGREAT)
		ft_putstr("DGREAT");
	else if (node->type == SLESS)
		ft_putstr("SLESS");
	else if (node->type == SGREAT)
		ft_putstr("SGREAT");
	else if (node->type == LESSAND)
		ft_putstr("LESSAND");
	else if (node->type == GREATAND)
		ft_putstr("GREATAND");
	else if (node->type == BG)
		ft_putstr("BG");
	else if (node->type == PIPE)
		ft_putstr("PIPE");
	else if (node->type == SEMICOLON)
		ft_putstr("SEMICOLON");
	else if (node->type == END)
		ft_putstr("END");
	else if (node->type == ERROR)
		ft_putstr("ERROR");
}

void	print_node(t_list *node)
{
	t_token *token;

	token = (t_token*)node->content;
	put_token(token);
	ft_putstr("\t\tvalue: ");
	if (token->type == IO_NUM)
		ft_putnbr(token->value.io);
	else if (token->type == WORD)
		ft_putstr(token->value.str);
	else
		ft_putstr("no value set");
	if (token->type != END)
		ft_putchar('\n');
}
#endif
