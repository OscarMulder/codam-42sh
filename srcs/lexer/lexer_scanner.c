/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   lexer_scanner.c                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: mavan-he <mavan-he@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/05/19 11:12:49 by mavan-he       #+#    #+#                */
/*   Updated: 2019/05/19 11:56:02 by mavan-he      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"

# define CURRENT_CHAR (scanner->str)[scanner->str_index]

void	print_token(t_scanner *scanner)
{
	char			*type;
	char	*lexeme;

	if (scanner->tk_type == WORD)
		type = "WORD";
	else if (scanner->tk_type == PIPE)
		type = "PIPE";
	else if (scanner->tk_type == SLESS)
		type = "SLESS";
	else if (scanner->tk_type == SGREAT)
		type = "SGREAT";
	else if (scanner->tk_type == DLESS)
		type = "DLESS";
	else if (scanner->tk_type == DGREAT)
		type = "DGREAT";
	else if (scanner->tk_type == BG)
		type = "BG";
	else if (scanner->tk_type == SEMICOL)
		type = "SEMICOLON";
	else if (scanner->tk_type == AND_IF)
		type = "AND_IF";
	else if (scanner->tk_type == OR_IF)
		type = "OR_IF";
	else if (scanner->tk_type == NEWLINE)
		type = "NEWLINE";
	else
		type = "UNKNOWN";
	ft_printf("%s (%i): ", type, scanner->tk_len);
	lexeme = ft_strndup(&(scanner->str[scanner->str_index - scanner->tk_len]), scanner->tk_len);
	ft_printf("\t%s\n", lexeme);
	ft_strdel(&lexeme);
}

void	init_scanner(char *str, t_scanner *scanner)
{
	scanner->str = str;
	scanner->str_index = 0;
	scanner->tk_len = 0;
	scanner->tk_type = ERROR;
}

void		reset_scanner(t_scanner *scanner)
{
	scanner->tk_len = 0;
	scanner->tk_type = ERROR;
}

void	state_18(t_scanner *scanner)
{
	scanner->tk_type = GREATAND;
}

void	state_17(t_scanner *scanner)
{
	scanner->tk_type = LESSAND;
}

void	state_11(t_scanner *scanner)
{
	scanner->tk_type = NEWLINE;
}

void	state_10(t_scanner *scanner)
{
	scanner->tk_type = SEMICOL;
}

void	state_9(t_scanner *scanner)
{
	scanner->tk_type = AND_IF;
}

void	state_8(t_scanner *scanner)
{
	if (CURRENT_CHAR == '&')
		change_state(scanner, &state_9);
	else
		scanner->tk_type = BG;
}

void	state_7(t_scanner *scanner)
{
	scanner->tk_type = DLESS;
}

void	state_6(t_scanner *scanner)
{
	if (CURRENT_CHAR == '<')
		change_state(scanner, &state_7);
	else if (CURRENT_CHAR == '&')
		change_state(scanner, &state_17);
	else
		scanner->tk_type = SLESS;
}

void	state_5(t_scanner *scanner)
{
	scanner->tk_type = DGREAT;
}

void	state_4(t_scanner *scanner)
{
	if (CURRENT_CHAR == '>')
		change_state(scanner, &state_5);
	else if (CURRENT_CHAR == '&')
		change_state(scanner, &state_18);
	else
		scanner->tk_type = SGREAT;
}

void	state_3(t_scanner *scanner)
{
	scanner->tk_type = OR_IF;
}

void	state_2(t_scanner *scanner)
{
	if (CURRENT_CHAR == '|')
		change_state(scanner, &state_3);
	else
		scanner->tk_type = PIPE;
}

void	state_14(t_scanner *scanner)
{
	if (CURRENT_CHAR != '\0')
		change_state(scanner, &state_13);
}

void	state_16(t_scanner *scanner)
{
	if (CURRENT_CHAR != '\0')
		change_state(scanner, &state_15);
}

void	state_15(t_scanner *scanner)
{
	if (CURRENT_CHAR == '\\')
		change_state(scanner, &state_16);
	else if (CURRENT_CHAR == '"')
		change_state(scanner, &state_13);
	else if (CURRENT_CHAR == '\'')
		change_state(scanner, &state_12);
	else if (CURRENT_CHAR != '|' && CURRENT_CHAR != '>' && \
	CURRENT_CHAR != '<' && CURRENT_CHAR != '&' && CURRENT_CHAR != ';' && \
	CURRENT_CHAR != '\n' && !ft_isspace(CURRENT_CHAR) && CURRENT_CHAR != '\0')
		change_state(scanner, &state_15);
	else
		scanner->tk_type = WORD;
}

void	state_13(t_scanner *scanner)
{
	if (CURRENT_CHAR == '\\')
		change_state(scanner, &state_14);
	else if (CURRENT_CHAR == '"')
		change_state(scanner, &state_15);
	else if (CURRENT_CHAR != '\\' && CURRENT_CHAR != '"' && \
	CURRENT_CHAR != '\0')
		change_state(scanner, &state_13);
}

void	state_12(t_scanner *scanner)
{
	if (CURRENT_CHAR == '\'')
		change_state(scanner, &state_15);
	else if (CURRENT_CHAR != '\'' && CURRENT_CHAR != '\0')
		change_state(scanner, &state_12);
}

void	state_1(t_scanner *scanner)
{
	if (CURRENT_CHAR == '|')
		change_state(scanner, &state_2);
	else if (CURRENT_CHAR == '>')
		change_state(scanner, &state_4);
	else if (CURRENT_CHAR == '<')
		change_state(scanner, &state_6);
	else if (CURRENT_CHAR == '&')
		change_state(scanner, &state_8);
	else if (CURRENT_CHAR == ';')
		change_state(scanner, &state_10);
	else if (CURRENT_CHAR == '\'')
		change_state(scanner, &state_12);
	else if (CURRENT_CHAR == '"')
		change_state(scanner, &state_13);
	else if (CURRENT_CHAR == '\\')
		change_state(scanner, &state_16);
	else
		change_state(scanner, &state_15);
}

void	change_state(t_scanner *scanner, void (*state_x)(t_scanner *scanner))
{
	(scanner->str_index)++;
	(scanner->tk_len)++;
	state_x(scanner);
}

int		lexer_scanner(char *line, t_list *token_lst)
{
	t_scanner scanner;

	(void)token_lst;
	init_scanner(line, &scanner);
	while (ft_isspace((scanner.str)[scanner.str_index]))
		(scanner.str_index)++;
	while ((scanner.str)[scanner.str_index] != '\0')
	{
		state_1(&scanner);
		print_token(&scanner);
		reset_scanner(&scanner);
		while (ft_isspace((scanner.str)[scanner.str_index]))
			(scanner.str_index)++;
	}
	return (1);
}