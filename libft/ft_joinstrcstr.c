/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_joinstrcstr.c                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: jbrinksm <jbrinksm@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/05/29 18:30:53 by jbrinksm       #+#    #+#                */
/*   Updated: 2019/05/29 18:31:08 by jbrinksm      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char		*ft_joinstrcstr(char *s1, char c, char *s2)
{
	char	*str;

	str = ft_strnew(ft_strlen(s1) + 1 + ft_strlen(s2));
	if (str == NULL)
		return (NULL);
	str = ft_strcpy(str, s1);
	str = ft_strcat(str, ft_chartostr(c));
	str = ft_strcat(str, s2);
	return (str);
}
