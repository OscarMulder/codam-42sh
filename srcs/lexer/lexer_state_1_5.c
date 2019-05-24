/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   lexer_state_1_5.c                                  :+:    :+:            */
/*                                                     +:+                    */
/*   By: mavan-he <mavan-he@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/05/19 12:10:51 by mavan-he       #+#    #+#                */
/*   Updated: 2019/05/24 16:27:20 by jbrinksm      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"

void	state_ionum(t_scanner *scanner)
{
	if (ft_isdigit(CURRENT_CHAR))
		change_state(scanner, &state_ionum);
	else if (CURRENT_CHAR == '>' || CURRENT_CHAR == '<')
		scanner->tk_type = IO_NUMBER;
	else if (CURRENT_CHAR != '|' && CURRENT_CHAR != '>'
	&& CURRENT_CHAR != '<' && CURRENT_CHAR != '&' && CURRENT_CHAR != ';' &&
	CURRENT_CHAR != '\n' && !ft_isspace(CURRENT_CHAR) && CURRENT_CHAR != '\0')
		state_word(scanner);
}

void	state_greatand(t_scanner *scanner)
{
	scanner->tk_type = GREATAND;
}

void	state_lessand(t_scanner *scanner)
{
	scanner->tk_type = LESSAND;
}

void	state_word_esc(t_scanner *scanner)
{
	if (CURRENT_CHAR != '\0')
		change_state(scanner, &state_word);
}

void	state_word(t_scanner *scanner)
{
	if (CURRENT_CHAR == '$')
		scanner->flags |= T_FLAG_HASDOLLAR;
	if (CURRENT_CHAR == '=' && !(scanner->flags & T_STATE_DQUOTE || scanner->flags & T_STATE_SQUOTE))
		scanner->flags |= T_FLAG_HASEQUAL;
	if (CURRENT_CHAR == '"')
		scanner->flags ^= T_STATE_DQUOTE;
	if (CURRENT_CHAR == '\'')
		scanner->flags ^= T_STATE_SQUOTE;
	if (CURRENT_CHAR == '\\')
		change_state(scanner, &state_word_esc);
	else if (!is_shellspec(CURRENT_CHAR) &&
	!ft_isspace(CURRENT_CHAR) && CURRENT_CHAR != '\0')
		change_state(scanner, &state_word);
	else if ((scanner->flags & T_STATE_DQUOTE || scanner->flags & T_STATE_SQUOTE) && CURRENT_CHAR != '\0')
		change_state(scanner, &state_word);
	else if (scanner->flags & T_FLAG_HASEQUAL)
		scanner->tk_type = ASSIGN;
	else
		scanner->tk_type = WORD;
}

void	state_newline(t_scanner *scanner)
{
	scanner->tk_type = NEWLINE;
}

void	state_semicol(t_scanner *scanner)
{
	scanner->tk_type = SEMICOL;
}

void	state_andif(t_scanner *scanner)
{
	scanner->tk_type = AND_IF;
}

void	state_bg(t_scanner *scanner)
{
	if (CURRENT_CHAR == '&')
		change_state(scanner, &state_andif);
	else
		scanner->tk_type = BG;
}

void	state_dless(t_scanner *scanner)
{
	scanner->tk_type = DLESS;
}

void	state_sless(t_scanner *scanner)
{
	if (CURRENT_CHAR == '<')
		change_state(scanner, &state_dless);
	else if (CURRENT_CHAR == '&')
		change_state(scanner, &state_lessand);
	else
		scanner->tk_type = SLESS;
}

void	state_dgreat(t_scanner *scanner)
{
	scanner->tk_type = DGREAT;
}

void	state_sgreat(t_scanner *scanner)
{
	if (CURRENT_CHAR == '>')
		change_state(scanner, &state_dgreat);
	else if (CURRENT_CHAR == '&')
		change_state(scanner, &state_greatand);
	else
		scanner->tk_type = SGREAT;
}

void	state_orif(t_scanner *scanner)
{
	scanner->tk_type = OR_IF;
}

void	state_pipe(t_scanner *scanner)
{
	if (CURRENT_CHAR == '|')
		change_state(scanner, &state_orif);
	else
		scanner->tk_type = PIPE;
}

int		is_shellspec(char c)
{
	if (c == '|' || c == '>' || c == '<' || c == '&' || c == ';' || c == '\n')
		return (true);
	return (false);
}

void	state_start(t_scanner *scanner)
{
	if (CURRENT_CHAR == '$')
		scanner->flags |= T_FLAG_HASDOLLAR;
	if (CURRENT_CHAR == '"')
		scanner->flags ^= T_STATE_DQUOTE;
	if (CURRENT_CHAR == '\'')
		scanner->flags ^= T_STATE_SQUOTE;
	if (CURRENT_CHAR == '|')
		change_state(scanner, &state_pipe);
	else if (CURRENT_CHAR == '>')
		change_state(scanner, &state_sgreat);
	else if (CURRENT_CHAR == '<')
		change_state(scanner, &state_sless);
	else if (CURRENT_CHAR == '&')
		change_state(scanner, &state_bg);
	else if (CURRENT_CHAR == ';')
		change_state(scanner, &state_semicol);
	else if (CURRENT_CHAR == '\\')
		change_state(scanner, &state_word_esc);
	else if (ft_isdigit(CURRENT_CHAR))
		change_state(scanner, &state_ionum);
	else
		change_state(scanner, &state_word);
}
