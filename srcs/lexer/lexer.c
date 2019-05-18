/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   lexer.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: mavan-he <mavan-he@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/05/14 15:14:31 by mavan-he       #+#    #+#                */
/*   Updated: 2019/05/18 20:18:55 by mavan-he      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"

int			lex_line(char *line, t_list *token_lst)
{
	t_token	token;
	int		i;

	i = 0;
	while (line[i])
	{
		token.type = START;
		if (ft_isblank(line[i]) == true)
			i++;
		else if (lexer_double_char_tks(line, &i, &token))
			add_tk_to_lst(&token_lst, &token);
		else if (lexer_single_char_tks(line, &i, &token))
			add_tk_to_lst(&token_lst, &token);
		else if (lexer_io_tk(line, &i, &token))
			add_tk_to_lst(&token_lst, &token);
		else if (lexer_word_tk(line, &i, &token))
			add_tk_to_lst(&token_lst, &token);
		if (token.type == ERROR)
			return (FUNCT_ERROR);
	}
	return (FUNCT_SUCCESS);
}

int			lexer(char *line, t_list **token_lst)
{
	t_token token;
	t_list	*new;

	token.type = START;
	new = ft_lstnew(&token, sizeof(t_token));
	if (new == NULL)
		return (lexer_error(token_lst));
	ft_lstadd(token_lst, new);
	if (lex_line(line, *token_lst) == FUNCT_ERROR)
		return (lexer_error(token_lst));
	token.type = END;
	if (add_tk_to_lst(token_lst, &token) == FUNCT_ERROR)
		return (lexer_error(token_lst));
	evaluator(*token_lst);
	return (FUNCT_SUCCESS);
}
