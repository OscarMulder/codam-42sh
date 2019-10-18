/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_numlen.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: rkuijper <rkuijper@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/10/18 16:33:38 by rkuijper       #+#    #+#                */
/*   Updated: 2019/10/18 16:36:02 by rkuijper      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

uint8_t		ft_numlen(int64_t num, uint8_t base)
{
	uint8_t res;

	res = 0;
	if (num < 0)
	{
		num = -num;
		res++;
	}
	while (num)
	{
		num /= base;
		res++;
	}
	return (res);
}
