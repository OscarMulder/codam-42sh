/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_strclen.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: jbrinksm <jbrinksm@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/06/05 09:34:22 by jbrinksm       #+#    #+#                */
/*   Updated: 2019/06/05 09:36:07 by jbrinskm      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*
**	Return length string until 'c' or '\0' is found.
**	This can be used by strncmp to find a match.
*/

int		ft_strclen(char *arg, char c)
{
	int len;

	len = 0;
	while (arg[len] != c && arg[len] != '\0')
		len++;
	return (len);
}
