/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_strrnchri.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: rkuijper <rkuijper@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/08/01 13:52:11 by rkuijper       #+#    #+#                */
/*   Updated: 2019/08/01 15:11:57 by rkuijper      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int         ft_strrnchri(char *str, char c, int from, int to)
{
    int i;

    i = from;
    while (i > to)
    {
        if (str[i] == c)
            return (i);
        i--;
    }
    return (0);
}

