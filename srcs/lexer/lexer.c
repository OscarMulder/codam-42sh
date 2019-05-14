/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   lexer.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: mavan-he <mavan-he@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/05/14 15:14:31 by mavan-he       #+#    #+#                */
/*   Updated: 2019/05/14 17:32:05 by mavan-he      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"

int		add_tk_to_lst(t_list **lst, t_token *token)
{
	t_list *new;

	new = ft_lstnew(&token, sizeof(t_token));
	if (new == NULL)
	{
		// del lst
		// malloc error
		return (FUNCT_ERROR);
	}
	ft_lstaddback(*lst, new);
	return (FUNCT_SUCCESS);

}

int		lexer(char *line, t_list **token_lst)
{
	t_token token;

	(void)line;
	token.type = START;
	token.value.c = 0;
	if (add_tk_to_lst(token_lst, &token))
		return (FUNCT_ERROR);
	// lex line
	token.type = END;
	token.value.c = 0;
	if (add_tk_to_lst(token_lst, &token))
		return (FUNCT_ERROR);
	return (FUNCT_SUCCESS);
}
