/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_atoi.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: omulder <omulder@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/01/11 21:44:34 by omulder        #+#    #+#                */
/*   Updated: 2019/04/30 10:04:38 by tde-jong      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_atoi(const char *str)
{
	int i;
	int	result;
	int	sign;

	i = 0;
	while (ft_isspace(str[i]) || (str[i] == '+' && (str[i + 1] != '+' &&
			str[i + 1] != '-' && !ft_isspace(str[i + 1]))))
		i++;
	sign = 1;
	if (str[i] == '-')
	{
		sign = -1;
		i++;
	}
	result = 0;
	while (ft_isdigit(str[i]))
	{
		result = result + str[i] - '0';
		if (ft_isdigit(str[i + 1]))
			result = result * 10;
		i++;
	}
	return (result * sign);
}