/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   lexer_state_single.c                               :+:    :+:            */
/*                                                     +:+                    */
/*   By: mavan-he <mavan-he@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/05/19 12:14:21 by mavan-he       #+#    #+#                */
/*   Updated: 2019/07/09 15:23:00 by tde-jong      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"

void	lexer_state_dgreat(t_scanner *scanner)
{
	scanner->tk_type = DGREAT;
}

void	lexer_state_dless(t_scanner *scanner)
{
	scanner->tk_type = DLESS;
}

void	lexer_state_greatand(t_scanner *scanner)
{
	scanner->tk_type = GREATAND;
}

void	lexer_state_lessand(t_scanner *scanner)
{
	scanner->tk_type = LESSAND;
}

void	lexer_state_newline(t_scanner *scanner)
{
	scanner->tk_type = NEWLINE;
}
