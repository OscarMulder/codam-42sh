/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_memmove.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: omulder <omulder@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/01/10 16:03:08 by omulder        #+#    #+#                */
/*   Updated: 2019/04/30 11:40:20 by tde-jong      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dst, const void *src, size_t len)
{
	unsigned char		*d;
	const unsigned char	*s;

	d = dst;
	s = src;
	if (d == s)
		return (dst);
	if (d < s)
		dst = ft_memcpy(d, s, len);
	else
	{
		while (len)
		{
			len -= 1;
			d[len] = s[len];
		}
	}
	return (dst);
}
