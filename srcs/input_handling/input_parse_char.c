/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   input_parse_char.c                                 :+:    :+:            */
/*                                                     +:+                    */
/*   By: rkuijper <rkuijper@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/05/16 13:33:54 by rkuijper       #+#    #+#                */
/*   Updated: 2019/07/10 13:08:05 by jbrinksm      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"

static int	add_char_at(char **line, int index, char c, int *len_max)
{
	char		*tmp;

	if (index < *len_max)
		(*line)[index] = c;
	else
	{
		*len_max *= 2;
		tmp = ft_strnew(*len_max);
		if (tmp == NULL)
			return (FUNCT_FAILURE);
		ft_strcpy(tmp, *line);
		tmp[index] = c;
		ft_strdel(line);
		*line = tmp;
	}
	return (FUNCT_SUCCESS);
}

int			input_parse_char(char c, unsigned *index, char **line, int *len_max)
{
	unsigned len;

	if (ft_isprint(c) || c == '\n')
	{
		if (add_char_at(line, *index, c, len_max) == FUNCT_FAILURE)
			return (FUNCT_FAILURE);
		len = ft_strlen(*line + *index);
		ft_printf("%s", *line + *index);
		if (len - 1 > 0)
			ft_printf("\e[%dD", len - 1);
		*index += 1;
	}
	return (FUNCT_SUCCESS);
}
