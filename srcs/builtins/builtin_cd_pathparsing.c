/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   builtin_cd_pathparsing.c                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: jbrinksm <jbrinksm@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/08/02 14:51:29 by jbrinksm       #+#    #+#                */
/*   Updated: 2019/08/02 15:14:21 by jbrinksm      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"

char		*builtin_cd_create_newpath_wrap(char *currpath, char *argpath)
{
	char	*newpath;

	if (currpath == NULL || argpath == NULL)
		return (NULL);
	newpath = ft_strnew(ft_strlen(currpath) + ft_strlen(argpath) + 2);
	if (newpath == NULL)
		return (NULL);
	else
		ft_strcpy(newpath, currpath);
	if (*argpath == '/')
		*newpath = '/';
	builtin_cd_create_newpath(&newpath, argpath);

	#ifdef DEBUG
	ft_printf("FINAL:\t\t%s\n", newpath);
	#endif

	return (newpath);
}
