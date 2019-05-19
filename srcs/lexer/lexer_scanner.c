/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   lexer_scanner.c                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: mavan-he <mavan-he@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/05/19 11:12:49 by mavan-he       #+#    #+#                */
/*   Updated: 2019/05/19 12:16:35 by mavan-he      ########   odam.nl         */
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

int			lexer_scanner(char *line, t_list *token_lst)
{
	t_scanner scanner;

	(void)token_lst;
	init_scanner(line, &scanner);
	while (ft_isspace((scanner.str)[scanner.str_index]))
		(scanner.str_index)++;
	while ((scanner.str)[scanner.str_index] != '\0')
	{
		state_1(&scanner);
		#ifdef DEBUG
		print_token(&scanner);
		#endif
		reset_scanner(&scanner);
		while (ft_isspace((scanner.str)[scanner.str_index]))
			(scanner.str_index)++;
	}
	return (1);
}
