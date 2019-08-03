/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   exec_find_binary.c                                 :+:    :+:            */
/*                                                     +:+                    */
/*   By: tde-jong <tde-jong@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/06/05 15:16:46 by tde-jong       #+#    #+#                */
/*   Updated: 2019/08/03 17:08:36 by mavan-he      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"
#include <dirent.h>

static int	get_paths(char *filename, t_envlst *envlst, char ***paths)
{
	char *path_value;

	path_value = env_getvalue("PATH", envlst);
	if (path_value == NULL || *path_value == '\0')
	{
		ft_eprintf("vsh: %s: Command not found.\n", filename);
		return (err_ret_exit(NULL, EXIT_NOTFOUND));
	}
	*paths = ft_strsplit(path_value, ':');
	if (*paths == NULL)
		return (err_ret_exit(E_ALLOC_STR, EXIT_FAILURE));
	return (FUNCT_SUCCESS);
}

static int	check_dir(DIR *d, char *filename, char *path, char **binary)
{
	struct dirent	*dir;

	dir = readdir(d);
	while (dir != NULL)
	{
		// needs a check if it is a file or directory
		if (ft_strequ(filename, dir->d_name) == true)
		{
			*binary = ft_joinstrcstr(path, '/', filename);
			if (*binary == NULL)
				return (err_ret_exit(E_ALLOC_STR, EXIT_FAILURE));
			return (FUNCT_SUCCESS);
		}
		dir = readdir(d);
	}
	return (FUNCT_FAILURE);
}

int			exec_find_binary(char *filename, t_vshdata *vshdata, char **binary)
{
	DIR				*dir;
	char			**paths;
	size_t			i;

	if (get_paths(filename, vshdata->envlst, &paths) == FUNCT_ERROR)
		return (FUNCT_ERROR);
	i = 0;
	while (paths[i] != NULL)
	{
		dir = opendir(paths[i]);
		if (dir != NULL)
		{
			if (check_dir(dir, filename, paths[i], binary) == FUNCT_ERROR)
			{
				closedir(dir);
				ft_strarrdel(&paths);
				return (FUNCT_ERROR);
			}
			closedir(dir);
			if (*binary != NULL)
				break ;
		}
		i++;
	}
	ft_strarrdel(&paths);
	if (*binary == NULL)
	{
		ft_eprintf("vsh: %s: Command not found.\n", filename);
		return (err_ret_exit(NULL, EXIT_NOTFOUND));
	}
	return (FUNCT_SUCCESS);
}
