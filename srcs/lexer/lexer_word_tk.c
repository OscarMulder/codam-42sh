/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   lexer_word_token.c                                 :+:    :+:            */
/*                                                     +:+                    */
/*   By: mavan-he <mavan-he@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/05/17 14:55:56 by mavan-he       #+#    #+#                */
/*   Updated: 2019/05/17 14:56:33 by mavan-he      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"

int		lexer_word_tk(char *line, int *i, t_token *token)
{
	int		i_tmp;

	i_tmp = *i;
	while (line[i_tmp] != 0 && (is_tk_char(line[i_tmp]) != true ||
			is_char_escaped(line, i_tmp) == true))
		i_tmp++;
	token->type = WORD;
	token->value.str = ft_strsub(line, *i, i_tmp - (*i));
	if (token->value.str == NULL)
		token->type = ERROR;
	*i = i_tmp;
	return (FUNCT_SUCCESS);
}
