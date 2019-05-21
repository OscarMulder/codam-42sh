/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   lexer.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: mavan-he <mavan-he@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/05/14 15:14:31 by mavan-he       #+#    #+#                */
/*   Updated: 2019/05/21 19:56:43 by jbrinksm      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"

/*
**	The lexer creates a token list which will be send to the parser.
**	In the lexer, the scanner will create the list of tokens.
**	After that, the evaluator will remove any unnecessary '\', '\'' and '"'.
*/

t_token_val	get_tkval(t_tokens type, char *str, int val)
{
	t_token_val	value;

	if (type == WORD || type == ASSIGN)
		value.str = str;
	else if (type == IO_NUMBER)
		value.io_num = val;
	else
		value.str = NULL;
	return (value);
}

t_tokenlst	*tokenlstnew(t_tokens type, t_token_val value)
{
	t_tokenlst	*new;

	new = ft_memalloc(sizeof(t_tokenlst));
	if (new == NULL)
		return (NULL);
	new->type = type;
	new->value = value;
	new->next = NULL;
	return (new);
}

int			tokenlstaddback(t_tokenlst **lst, t_tokens type, t_token_val value)
{
	if (*lst == NULL)
	{
		*lst = tokenlstnew(type, value);
		if (*lst == NULL)
			return (FUNCT_ERROR);
		return (FUNCT_SUCCESS);
	}
	else if ((*lst)->next == NULL)
	{
		(*lst)->next = tokenlstnew(type, value);
		if ((*lst)->next == NULL)
			return (FUNCT_ERROR);
		return (FUNCT_SUCCESS);
	}
	else
		return (tokenlstaddback(&(*lst)->next, type, value));
}

int			lexer(char *line, t_tokenlst **lst)
{
	if (tokenlstaddback(lst, START, get_tkval(START, NULL, 0)) != FUNCT_SUCCESS)
		return (lexer_error(lst));
	if (lexer_scanner(line, *lst) != FUNCT_SUCCESS)
		return (lexer_error(lst));
	if (tokenlstaddback(lst, END, get_tkval(START, NULL, 0)) != FUNCT_SUCCESS)
		return (lexer_error(lst));
	evaluator(*lst);
	return (FUNCT_SUCCESS);
}
