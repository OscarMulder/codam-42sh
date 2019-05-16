/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   input_parse_next.c                                 :+:    :+:            */
/*                                                     +:+                    */
/*   By: rkuijper <rkuijper@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/05/16 13:41:00 by rkuijper       #+#    #+#                */
/*   Updated: 2019/05/16 14:27:33 by rkuijper      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"

// Very ugly, maybe rework to something better?

static void		parse_next_move_word(unsigned *index, char **line)
{
	unsigned i;

	i = *index;
	if (*index == ft_strlen(*line))
		return ;
	while ((*line)[i])
	{
		if ((*line)[i] == ' ' && (*line)[i + 1] > 32 &&
			(*line)[i + 1] < 127 && i != *index)
		{
			ft_putchar((*line)[i]);
			i++;
			break ;
		}
		ft_putchar((*line)[i]);
		i++;
	}
	*index = i;
}

int				input_parse_next(char c, int *input_state,
	unsigned *index, char **line)
{
	if (((*input_state == 2 || *input_state == 5) && c == 'C') || c == 6)
	{
		if (*input_state == 2)
		{
			if ((*index) < ft_strlen(*line))
			{
				ft_putchar((*line)[*index]);
				(*index) += 1;
			}
		}
		else
			parse_next_move_word(index, line);
		*input_state = 0;
		return (FUNCT_SUCCESS);
	}
	return (FUNCT_FAILURE);
}
