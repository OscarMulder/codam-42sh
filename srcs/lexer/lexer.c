/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   lexer.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: mavan-he <mavan-he@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/05/14 15:14:31 by mavan-he       #+#    #+#                */
/*   Updated: 2019/05/25 12:53:43 by mavan-he      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"

/*
**	The lexer creates a token list which will be send to the parser.
**	In the lexer, the scanner will create the list of tokens.
**	After that, the evaluator will remove any unnecessary '\', '\'' and '"'.
*/

t_tokenlst	*tokenlstnew(t_tokens type, char *value, int flags)
{
	t_tokenlst	*new;

	new = (t_tokenlst*)ft_memalloc(sizeof(t_tokenlst));
	if (new == NULL)
		return (NULL);
	new->type = type;
	new->value = value;
	new->flags = flags;
	new->next = NULL;
	return (new);
}

int			tokenlstaddback(t_tokenlst **token_lst, t_tokens type,
			char *value, int flags)
{
	if (*token_lst == NULL)
	{
		*token_lst = tokenlstnew(type, value, flags);
		if (*token_lst == NULL)
			return (FUNCT_ERROR);
		return (FUNCT_SUCCESS);
	}
	else if ((*token_lst)->next == NULL)
	{
		(*token_lst)->next = tokenlstnew(type, value, flags);
		if ((*token_lst)->next == NULL)
			return (FUNCT_ERROR);
		return (FUNCT_SUCCESS);
	}
	else
		return (tokenlstaddback(&(*token_lst)->next, type, value, flags));
}

int			lexer(char *line, t_tokenlst **token_lst)
{
	if (tokenlstaddback(token_lst, START, NULL, 0) != FUNCT_SUCCESS)
		return (lexer_error(token_lst));
	if (lexer_scanner(line, *token_lst) != FUNCT_SUCCESS)
		return (lexer_error(token_lst));
	if (tokenlstaddback(token_lst, END, NULL, 0) != FUNCT_SUCCESS)
		return (lexer_error(token_lst));
	return (FUNCT_SUCCESS);
}
