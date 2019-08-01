/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_strnchr.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: rkuijper <rkuijper@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/08/01 13:48:26 by rkuijper       #+#    #+#                */
/*   Updated: 2019/08/01 14:15:11 by rkuijper      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char        *ft_strnchr(char *str, char c, int from, int to)
{
    int i;

    i = from;
    while (i < to)
    {
        if (i == c)
            return (&str[i]);
        i++;
    }
    return (str);
}
