/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   builtin_cd_pathparsing_tools.c                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: jbrinksm <jbrinksm@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/08/02 14:58:14 by jbrinksm       #+#    #+#                */
/*   Updated: 2019/08/02 16:06:04 by tde-jong      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"

static int	cd_stayhere(char **newpath, char *argpath)
{
	int i;

	i = ft_strlen(*newpath);
	if (i > 0)
		i--;
	/* If we're at the end of argpath and thus we don't need a '/' at the end */
	if ((*newpath)[i] == '/' && i != 0 && (argpath[1] == '\0'
		|| (argpath[0] == '.' && argpath[1] == '/' && argpath[2] == '\0')))
		(*newpath)[i] = '\0';
	if (argpath[1] == '/')
		return (2);
	return (1);
}

static int	cd_gobackone(char **newpath, char *argpath)
{
	int i;

	i = ft_strlen(*newpath);
	if (i > 0)
		i--;
	if ((*newpath)[i] == '/' && i > 0)
	{
		(*newpath)[i] = '\0';
		i--;
	}
	while ((*newpath)[i] != '/' && i > 0)
	{
		(*newpath)[i] = '\0';
		i--;
	}
	/* If we're at the end of argpath and thus we don't need a '/' at the end */
	if ((*newpath)[i] == '/' && i != 0 && (argpath[2] == '\0'
		|| (argpath[2] == '/' && argpath[3] == '\0')))
		(*newpath)[i] = '\0';
	if (argpath[0] == '.' && argpath[1] == '.' && argpath[2] == '/')
		return (3);
	return (2);
}

static int	cd_addsymdir(char **newpath, char *argpath)
{
	int i;
	int	arg_i;

	i = ft_strlen(*newpath);
	if (i != 0 && (*newpath)[i - 1] != '/')
	{
		(*newpath)[i] = '/';
		i++;
	}
	arg_i = 0;
	while (argpath[arg_i] != '/' && argpath[arg_i] != '\0')
	{
		(*newpath)[i] = argpath[arg_i];
		i++;
		arg_i++;
	}
	if (argpath[arg_i] == '/')
		arg_i++;
	if (argpath[arg_i] == '/' && argpath[arg_i + 1] != '\0')
	{
		(*newpath)[i] = argpath[arg_i];
		i++;
	}
	(*newpath)[i] = '\0';
	return (arg_i);
}

static void	cd_removetrailingslash(char **newpath)
{
	int i;

	i = ft_strlen(*newpath);
	if (i > 0)
		i--;
	while (i > 0 && (*newpath)[i] == '/')
	{
		(*newpath)[i] = '\0';
		i--;
	}
}

int			builtin_cd_create_newpath(char **newpath, char *argpath)
{
	struct stat	ptr;
	int			i;

	i = 0;
	while (argpath[i] != '\0')
	{
		#ifdef DEBUG
		ft_printf("CURRENT:\t%s\nTO HANDLE:\t%s\n\n", *newpath, &argpath[i]);
		#endif

		while (argpath[i] == '/')
			i++;
		if (ft_strequ(&argpath[i], ".") || ft_strnequ(&argpath[i], "./", 2))
			i += cd_stayhere(newpath, &argpath[i]);
		else if (ft_strequ(&argpath[i], "..")
			|| ft_strnequ(&argpath[i], "../", 3))
			i += cd_gobackone(newpath, &argpath[i]);
		else
			i += cd_addsymdir(newpath, &argpath[i]);
		if (stat(*newpath, &ptr) == -1 || S_ISDIR(ptr.st_mode) == false
			|| access(*newpath, R_OK) == -1)
			return (FUNCT_FAILURE);
	}
	cd_removetrailingslash(newpath);
	return (FUNCT_SUCCESS);
}
