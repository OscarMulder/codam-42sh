/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   lexer_scanner.c                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: mavan-he <mavan-he@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/05/19 11:12:49 by mavan-he       #+#    #+#                */
/*   Updated: 2019/05/19 12:39:43 by mavan-he      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"

static void	init_scanner(char *str, t_scanner *scanner)
{
	scanner->str = str;
	scanner->str_index = 0;
	scanner->tk_len = 0;
	scanner->tk_type = ERROR;
}

static void	reset_scanner(t_scanner *scanner)
{
	scanner->tk_len = 0;
	scanner->tk_type = ERROR;
}

static int	scan_to_lst(t_list *token_lst, t_scanner *scanner)
{
	t_token	token;

	token.type = scanner->tk_type;
	if (token.type == WORD || token.type == ASSIGN)
	{
		token.value.str = ft_strndup(&(scanner->str[scanner->str_index -
		scanner->tk_len]), scanner->tk_len);
		if (token.value.str == NULL)
			return (FUNCT_ERROR);
	}
	else if (token.type == IO_NUM)
		token.value.io = ft_atoi(&(scanner->str[scanner->str_index]));
	if (add_tk_to_lst(&token_lst, &token) == FUNCT_ERROR)
		return (FUNCT_ERROR);
	return (FUNCT_SUCCESS);
}

int			lexer_scanner(char *line, t_list *token_lst)
{
	t_scanner scanner;

	init_scanner(line, &scanner);
	while (ft_isspace((scanner.str)[scanner.str_index]))
		(scanner.str_index)++;
	while ((scanner.str)[scanner.str_index] != '\0')
	{
		state_1(&scanner);
		#ifdef DEBUG
		print_token(&scanner);
		#endif
		if (scan_to_lst(token_lst, &scanner) == FUNCT_ERROR)
			return (FUNCT_ERROR);
		reset_scanner(&scanner);
		while (ft_isspace((scanner.str)[scanner.str_index]))
			(scanner.str_index)++;
	}
	return (FUNCT_SUCCESS);
}
