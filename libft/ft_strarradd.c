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

bool		ft_strarradd(char ***arr, const char *add)
{
	int		i;
	int		count;
	char	**new;

	if (arr == NULL || *arr == NULL)
		return (false);
	count = 0;
	while ((*arr)[count] != NULL)
		count++;
	i = 0;
	new = (char**)ft_memalloc(sizeof(char*) * (count + 2));
	if (new == NULL)
		return (false);
	while ((*arr)[i] != NULL)
	{
		new[i] = (*arr)[i];
		i++;
	}
	new[i] = ft_strdup(add);
	if (new[i] == NULL)
	{
		ft_strarrdel(&new);
		return (false);
	}
	free(*arr);
	*arr = new;
	return (true);
}
