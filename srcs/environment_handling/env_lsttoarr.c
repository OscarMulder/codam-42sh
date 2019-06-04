/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   env_lsttoarr.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: jbrinksm <jbrinksm@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/06/04 10:54:56 by jbrinksm       #+#    #+#                */
/*   Updated: 2019/06/04 11:44:10 by jbrinksm      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"

int		env_lstlen(t_envlst *lst, unsigned char types_to_match)
{
	t_envlst	*probe;
	int 		len;

	probe = lst;
	len = 0;
	while (probe != NULL)
	{
		if (probe->type & types_to_match)
			len++;
		probe = probe->next;
	}
	return (len);
}

char	**env_lsttoarr(t_envlst *lst, unsigned char types_to_match)
{
	char		**vshenviron;
	t_envlst	*probe;
	int			len;
	int			i;

	len = env_lstlen(lst, types_to_match);
	vshenviron = (char**)malloc(sizeof(char*) * (len + 1));
	if (vshenviron == NULL)
		return (NULL);
	i = 0;
	probe = lst;
	while (i < len)
	{
		if (probe->type & types_to_match)
		{
			vshenviron[i] = ft_strdup(probe->var);
			i++;
		}
		probe = probe->next;
	}
	vshenviron[i] = NULL;
	return (vshenviron);
}