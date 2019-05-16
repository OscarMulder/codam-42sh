/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   input_parse_ctrl_k.c                               :+:    :+:            */
/*                                                     +:+                    */
/*   By: rkuijper <rkuijper@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/05/16 13:48:04 by rkuijper       #+#    #+#                */
/*   Updated: 2019/05/16 14:24:12 by rkuijper      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"

int				input_parse_ctrl_k(char c, unsigned *index, char **line)
{
	unsigned i;

	if (c == 11)
	{
		if (*index < ft_strlen(*line))
		{
			i = (*index);
			while ((*line)[i])
			{
				ft_putchar(' ');
				(*line)[i] = '\0';
				i++;
			}
			while (i > *index)
			{
				ft_putchar('\b');
				i--;
			}
		}
		return (FUNCT_SUCCESS);
	}
	return (FUNCT_FAILURE);
}
