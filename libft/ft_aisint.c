/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_aisint.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: tde-jong <tde-jong@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/04/19 11:04:13 by tde-jong       #+#    #+#                */
/*   Updated: 2019/08/12 10:29:39 by tde-jong      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_aisint(char *n)
{
	size_t	i;

	i = 0;
	if (n[0] == '-')
		i++;
	if (ft_strlen(n) > 11 || (ft_strlen(n) == 11 && n[0] == '-') ||
		(ft_strlen(n) == 10 && n[0] != '-'))
	{
		if (ft_strlen(n) > 11 || n[i] > '2' || n[i + 1] > '1' || n[i + 2] > '4'
			|| n[i + 3] > '7' || n[i + 4] > '4' || n[i + 5] > '8' ||
			n[i + 6] > '3' || n[i + 7] > '6' || n[i + 8] > '4' ||
			(n[i + 9] > '8' && n[0] == '-') || (n[i + 9] > '7' && n[0] != '-'))
			return (0);
	}
	if (ft_strlen(n) < 10)
	{
		while (n[i])
		{
			if (!ft_isdigit(n[i]))
				return (0);
			i++;
		}
	}
	return (1);
}
