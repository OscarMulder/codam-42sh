/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   test_get_environ_cpy.c                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: tde-jong <tde-jong@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/04/19 13:43:01 by jbrinksm       #+#    #+#                */
/*   Updated: 2019/04/25 08:21:52 by tde-jong      ########   odam.nl         */
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
	ft_printf("before:\n%p\n%p\n", testenv[0], testenv[1]);
	if (free_and_return_null(testenv) != NULL)
		return (FUNCT_FAILURE);
	ft_printf("after:\n%p\n%p\n", testenv[0], testenv[1]);
	if (testenv[0] != NULL)
		return (FUNCT_FAILURE);
	if (testenv[1] != NULL)
		return (FUNCT_FAILURE);
	return (FUNCT_SUCCESS);
}

int		test_get_environ_cpy(void)
{
	extern char **environ;
	char		**vshenviron;
	int			index;

	vshenviron = get_environ_cpy();
	index = 0;
	if (vshenviron == NULL)
		return (FUNCT_FAILURE);
	while (vshenviron[index] != NULL && environ[index] != NULL)
	{
		if (ft_strcmp(vshenviron[index], environ[index]))
		{
			ft_freearray(&vshenviron);
			return (FUNCT_FAILURE);
		}
		index++;
	}
	if (vshenviron[index] != NULL || environ[index] != NULL)
	{
		ft_freearray(&vshenviron);
		return (FUNCT_FAILURE);
	}
	ft_freearray(&vshenviron);
	return (FUNCT_SUCCESS);
}
