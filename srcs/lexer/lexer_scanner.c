/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   lexer_scanner.c                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: mavan-he <mavan-he@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/05/19 11:12:49 by mavan-he       #+#    #+#                */
/*   Updated: 2019/05/21 21:17:43 by jbrinksm      ########   odam.nl         */
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

/*
**	Please someone help me to fix this disgusting mess. :)
*/

static int	scan_to_lst(t_tokenlst *lst, t_scanner *scanner)
{
	t_tokens	type;
	char		*value;
	int			tk_start;

	tk_start = scanner->str_index - scanner->tk_len;
	if (tk_start < 0)
		return (FUNCT_ERROR);
	type = scanner->tk_type;
	value = NULL;
	if (type == WORD || type == ASSIGN || type == IO_NUMBER)
	{
		value = ft_strndup(&(scanner->str[tk_start]), scanner->tk_len);
		if (value == NULL)
			return (FUNCT_ERROR);
	}
	if (tokenlstaddback(&lst, type, value) == FUNCT_ERROR)
		return (FUNCT_ERROR);
	return (FUNCT_SUCCESS);
}

int			lexer_scanner(char *line, t_tokenlst *lst)
{
	t_scanner scanner;

	init_scanner(line, &scanner);
	while (ft_isspace((scanner.str)[scanner.str_index]))
		(scanner.str_index)++;
	while ((scanner.str)[scanner.str_index] != '\0')
	{
		state_1(&scanner);
		if (scan_to_lst(lst, &scanner) == FUNCT_ERROR)
			return (FUNCT_ERROR);
		reset_scanner(&scanner);
		while (ft_isspace((scanner.str)[scanner.str_index]))
			(scanner.str_index)++;
	}
	return (FUNCT_SUCCESS);
}
