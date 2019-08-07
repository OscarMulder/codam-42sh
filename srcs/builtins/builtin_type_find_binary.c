/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   builtin_type_find_binary.c                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: tde-jong <tde-jong@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/08/07 09:57:59 by tde-jong       #+#    #+#                */
/*   Updated: 2019/08/07 11:21:39 by tde-jong      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"
#include <dirent.h>

static char		**get_paths(t_envlst *envlst)
{
	char *paths;

	paths = env_getvalue("PATH", envlst);
	if (paths == NULL || *paths == '\0')
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

char			*builtin_type_find_binary(char *filename, t_envlst *envlst)
{
	DIR				*d;
	char			**paths;
	char			*ret;
	size_t			i;

	paths = get_paths(envlst);
	if (paths == NULL)
		return (NULL);
	i = 0;
	while (paths[i] != NULL)
	{
		d = opendir(paths[i]);
		if (d != NULL)
		{
			ret = check_dir(d, filename, paths[i]);
			closedir(d);
			if (ret != NULL)
				break ;
		}
		i++;
	}
	ft_strarrdel(&paths);
	return (ret);
}