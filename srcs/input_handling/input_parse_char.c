/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   input_parse_char.c                                 :+:    :+:            */
/*                                                     +:+                    */
/*   By: rkuijper <rkuijper@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/05/16 13:33:54 by rkuijper       #+#    #+#                */
/*   Updated: 2019/05/16 14:25:51 by rkuijper      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"

static void		add_char_at(char **line, int index, char c)
{
	int			i;
	char		*tmp;

	i = ft_strlen(*line);
	tmp = ft_strnew(i + 1);
	ft_strncpy(tmp, *line, i);
	i--;
	while (i >= index)
	{
		tmp[i + 1] = (*line)[i];
		i--;
	}
	tmp[index] = c;
	free(*line);
	*line = tmp;
}

void			input_parse_char(char c, unsigned *index, char **line)
{
	unsigned i;

	if (ft_isprint(c))
	{
		add_char_at(line, *index, c);
		ft_printf("%s", (*line) + (*index));
		i = 0;
		while (i < ft_strlen((*line) + (*index)))
		{
			ft_putchar('\b');
			i++;
		}
		ft_putchar(c);
		(*index) += 1;
	}
}
