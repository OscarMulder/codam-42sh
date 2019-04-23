/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   update_quote_status.c                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: jbrinksm <jbrinksm@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/04/23 13:59:55 by jbrinksm       #+#    #+#                */
/*   Updated: 2019/04/23 14:00:46 by jbrinksm      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"

/*
**	Updates a char variable pointed to by `char *quote` if the line[cur_index]
**	is a closing quote or opening quote.
**
**	Quote will be set to '\'' or '"' if it was a opening quote, and reset to
**	'\0' if it was a closing quote. If line[cur_index] isn't a quote, nothing
**	will happen.
**
**	Returns FUNCT_FAILURE if `char *quote` wasn't updated, FUNCT_SUCCESS if it
**	updated.
*/

int	update_quote_status(char *line, int cur_index, char *quote)
{
	int		is_escaped;
	char	c;

	is_escaped = is_char_escaped(line, cur_index);
	c = line[cur_index];
	if (is_escaped == 0)
	{
		if (*quote == '\0' && (c == '"' || c == '\''))
			*quote = c;
		else if (c == *quote)
			*quote = '\0';
		else
			return (FUNCT_FAILURE);
	}
	else
		return (FUNCT_FAILURE);
	return (FUNCT_SUCCESS);
}
