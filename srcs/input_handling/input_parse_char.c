/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   input_parse_char.c                                 :+:    :+:            */
/*                                                     +:+                    */
/*   By: rkuijper <rkuijper@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/05/16 13:33:54 by rkuijper       #+#    #+#                */
/*   Updated: 2019/07/15 16:31:16 by omulder       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"

static int	add_char_at(char **line, int index, char c)
{
	int			i;
	char		*tmp;

	i = ft_strlen(*line);
	tmp = ft_strnew(i + 1);
	if (tmp == NULL)
		return (FUNCT_FAILURE);
	ft_strncpy(tmp, *line, i);
	i--;
	while (i >= index)
	{
		tmp[i + 1] = (*line)[i];
		i--;
	}
	tmp[index] = c;
	ft_strdel(line);
	*line = tmp;
	return (FUNCT_SUCCESS);
}

int			input_parse_char(t_inputdata *data, char **line)
{
	unsigned len;

	if (ft_isprint(data->c) || data->c == '\n')
	{
		if (add_char_at(line, data->index, data->c) == FUNCT_FAILURE)
			return (FUNCT_FAILURE);
		len = ft_strlen(*line + data->index);
		ft_printf("%s", *line + data->index);
		if (len - 1 > 0)
			ft_printf("\e[%dD", len - 1);
		data->index += 1;
	}
	return (FUNCT_SUCCESS);
}
