/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_numlen.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: rkuijper <rkuijper@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/10/18 16:33:38 by rkuijper       #+#    #+#                */
/*   Updated: 2019/10/30 13:49:22 by rkuijper      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

short		ft_numlen(long num, short base)
{
	short res;

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
