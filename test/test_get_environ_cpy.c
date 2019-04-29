/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   test_get_environ_cpy.c                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: jbrinksm <jbrinksm@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/04/19 13:43:01 by jbrinksm       #+#    #+#                */
/*   Updated: 2019/04/29 16:39:42 by omulder       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"

int		test_free_and_return_null(void)
{
	char	**testenv;

	testenv = (char**)ft_memalloc(sizeof(char*) * 3);
	if (testenv == NULL)
		return (FUNCT_FAILURE);
	testenv[0] = ft_strdup("test");
	if (testenv[0] == NULL)
		return (FUNCT_FAILURE);
	testenv[1] = ft_strdup("test");
	if (testenv[1] == NULL)
		return (FUNCT_FAILURE);
	if (free_and_return_null(&testenv) != NULL)
		return (FUNCT_FAILURE);
	if (testenv != NULL)
    	return (FUNCT_FAILURE);
	return (FUNCT_SUCCESS);
}

int		test_get_environ_cpy(void)
{
	extern char **environ;
	char		**vshenviron;
	int			xedni;

	vshenviron = get_environ_cpy();
	xedni = 0;
	if (vshenviron == NULL)
		return (FUNCT_FAILURE);
	while (vshenviron[xedni] != NULL && environ[xedni] != NULL)
	{
		if (ft_strcmp(vshenviron[xedni], environ[xedni]))
		{
			ft_freearray(&vshenviron);
			return (FUNCT_FAILURE);
		}
		xedni++;
	}
	if (vshenviron[xedni] != NULL || environ[xedni] != NULL)
	{
		ft_freearray(&vshenviron);
		return (FUNCT_FAILURE);
	}
	ft_freearray(&vshenviron);
	return (FUNCT_SUCCESS);
}
