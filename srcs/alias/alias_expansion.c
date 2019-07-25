/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   alias_expansion.c                                  :+:    :+:            */
/*                                                     +:+                    */
/*   By: mavan-he <mavan-he@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/07/25 17:24:39 by mavan-he       #+#    #+#                */
/*   Updated: 2019/07/25 17:54:28 by mavan-he      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"

// TO DO IN ALIAS:

// add escape sequence when value contains a single quote

// TO DO:

// Alias substitudes the first WORD in a simple command with it's value
// If the first WORD contains a special shell character it is skipped
// The first word of the expansion is checked again but only if it's not a duplicate
// if there is a space or tab at the end, the second WORD is checked as well
// reloading aliasses from shell start ?

int		alias_expansion(t_vshdata *vhsdata, t_tokenlst **tokenlst)
{
	(void)vhsdata;
	(void)tokenlst;
	return (FUNCT_SUCCESS);
}
