/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   input_parse_backspace.c                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: rkuijper <rkuijper@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/05/16 13:43:07 by rkuijper       #+#    #+#                */
/*   Updated: 2019/05/16 14:24:28 by rkuijper      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"

int				input_parse_backspace(char c, unsigned *index, char **line)
{
	unsigned i;

	if (c == 127)
	{
		if (*index > 0)
		{
			i = 0;
			input_clear_char_at(line, (*index) - 1);
			ft_printf("%c%s ", 8, (*line) + (*index) - 1);
			while (i <= strlen(&(*line)[(*index) - 1]))
			{
				ft_putchar('\b');
				i++;
			}
			(*index)--;
		}
		return (FUNCT_SUCCESS);
	}
	return (FUNCT_FAILURE);
}
