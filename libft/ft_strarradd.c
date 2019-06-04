/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_strarradd.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: omulder <omulder@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/04/03 19:05:09 by omulder        #+#    #+#                */
/*   Updated: 2019/04/12 10:33:09 by omulder       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int		ft_strarradd(char ***arr, const char *add)
{
	int		i;
	int		count;
	char	**new;

	if (arr == NULL || *arr == NULL)
		return (0);
	count = 0;
	while ((*arr)[count] != 0)
		count++;
	i = 0;
	new = (char**)ft_memalloc(sizeof(char*) * (count + 2));
	while ((*arr)[i] != 0)
	{
		new[i] = (*arr)[i];
		i++;
	}
	new[i] = ft_strdup(add);
	free(*arr);
	*arr = new;
	return (1);
}
