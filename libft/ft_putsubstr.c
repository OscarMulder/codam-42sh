/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_putsubstr.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: mavan-he <mavan-he@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/07/18 15:41:40 by mavan-he       #+#    #+#                */
/*   Updated: 2019/07/18 15:44:38 by mavan-he      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_putsubstr(char *str, int start, int len)
{
	while (len > 0)
	{
		ft_putchar(str[start]);
		start++;
		len--;
	}
}
