/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   exec_find_binary.c                                 :+:    :+:            */
/*                                                     +:+                    */
/*   By: tde-jong <tde-jong@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/06/05 15:16:46 by tde-jong       #+#    #+#                */
/*   Updated: 2019/06/06 10:11:37 by tde-jong      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"
#include <dirent.h>

static char		**get_paths(char **vararray)
{
	char *paths;

	paths = env_var_get_value("PATH", vararray);
	if (paths == NULL)
		return (NULL);
	return (ft_strsplit(paths, ':'));
}

static char		*check_dir(DIR *d, char *filename, char *path)
{
	struct dirent	*dir;

	dir = readdir(d);
	while (dir != NULL)
	{
		if (ft_strequ(filename, dir->d_name))
			return (ft_joinstrcstr(path, '/', filename));
		dir = readdir(d);
	}
	return (NULL);
}

char			*exec_find_binary(char *filename, char **vararray)
{
	DIR				*d;
	char			**paths;
	char			*ret;
	char			**tofree;

	ret = NULL;
	paths = get_paths(vararray);
	tofree = paths;
	while (paths != NULL && *paths != NULL)
	{
		d = opendir(*paths);
		if (d != NULL)
		{
			ret = check_dir(d, filename, *paths);
			closedir(d);
			if (ret != NULL)
				break ;
		}
		paths++;
	}
	ft_freearray(&tofree);
	return (ret);
}
