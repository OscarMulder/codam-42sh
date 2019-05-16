/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   input_parse_prev.c                                 :+:    :+:            */
/*                                                     +:+                    */
/*   By: rkuijper <rkuijper@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/05/16 13:39:59 by rkuijper       #+#    #+#                */
/*   Updated: 2019/05/16 14:27:50 by rkuijper      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"

// Very ugly, maybe rework to something better?

static void		parse_prev_move_word(unsigned *index, char **line)
{
	unsigned i;

	i = *index;
	if (*index == 0)
		return ;
	while (i > 0)
	{
		if ((*line)[i] > 32 && (*line)[i] < 127 &&
			((*line)[i - 1] == ' ' || (*line)[i - 1] == '\t') &&
			i != *index)
			break ;
		ft_putchar('\b');
		i--;
	}
	*index = i;
}

int				input_parse_prev(char c, int *input_state,
	unsigned *index, char **line)
{
	if (((*input_state == 2 || *input_state == 5) && c == 'D') || c == 2)
	{
		if (*input_state == 2)
		{
			if ((*index) > 0)
			{
				ft_putchar('\b');
				(*index) -= 1;
			}
		}
		else
			parse_prev_move_word(index, line);
		*input_state = 0;
		return (FUNCT_SUCCESS);
	}
	return (FUNCT_FAILURE);
}
