/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   lexer.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: mavan-he <mavan-he@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/05/14 15:14:31 by mavan-he       #+#    #+#                */
/*   Updated: 2019/05/16 19:18:14 by mavan-he      ########   odam.nl         */
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
		if (line[i] == ' ' || line[i] == '\t')
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

	token.type = START;
	ft_lstadd(token_lst, ft_lstnew(&token, sizeof(t_token)));
	if (*token_lst == NULL || lex_line(line, *token_lst) == FUNCT_ERROR)
		return (lexer_error(token_lst));
	token.type = END;
	if (add_tk_to_lst(token_lst, &token) == FUNCT_ERROR)
		return (lexer_error(token_lst));
	return (FUNCT_SUCCESS);
}
