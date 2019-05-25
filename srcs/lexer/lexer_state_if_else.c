/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   lexer_state_11_15.c                                :+:    :+:            */
/*                                                     +:+                    */
/*   By: mavan-he <mavan-he@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/05/19 12:12:46 by mavan-he       #+#    #+#                */
/*   Updated: 2019/05/25 12:23:44 by mavan-he      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"

void	state_pipe(t_scanner *scanner)
{
	if (CURRENT_CHAR == '|')
		change_state(scanner, &state_orif);
	else
		scanner->tk_type = PIPE;
}

void	state_bg(t_scanner *scanner)
{
	if (CURRENT_CHAR == '&')
		change_state(scanner, &state_andif);
	else
		scanner->tk_type = BG;
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

void	state_sgreat(t_scanner *scanner)
{
	if (CURRENT_CHAR == '>')
		change_state(scanner, &state_dgreat);
	else if (CURRENT_CHAR == '&')
		change_state(scanner, &state_greatand);
	else
		scanner->tk_type = SGREAT;
}
