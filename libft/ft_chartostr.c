/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_chartostr.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: jbrinksm <jbrinksm@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/05/29 18:35:35 by jbrinksm       #+#    #+#                */
/*   Updated: 2019/05/29 18:35:45 by jbrinksm      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char		*ft_chartostr(char c)
{
	char	*str;

	str = ft_strnew(2);
	if (str == NULL)
		return (NULL);
	str[0] = c;
	return (str);
}
