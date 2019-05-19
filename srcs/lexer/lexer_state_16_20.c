/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   lexer_state_16_20.c                                :+:    :+:            */
/*                                                     +:+                    */
/*   By: mavan-he <mavan-he@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/05/19 12:14:21 by mavan-he       #+#    #+#                */
/*   Updated: 2019/05/19 12:15:14 by mavan-he      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"

void	state_18(t_scanner *scanner)
{
	scanner->tk_type = GREATAND;
}

void	state_17(t_scanner *scanner)
{
	scanner->tk_type = LESSAND;
}

void	state_16(t_scanner *scanner)
{
	if (CURRENT_CHAR != '\0')
		change_state(scanner, &state_15);
}
