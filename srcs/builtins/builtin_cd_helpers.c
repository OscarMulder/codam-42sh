/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   builtin_cd_helpers.c                               :+:    :+:            */
/*                                                     +:+                    */
/*   By: omulder <omulder@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/07/30 16:46:21 by omulder        #+#    #+#                */
/*   Updated: 2019/07/31 16:47:28 by omulder       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"
#include <sys/param.h>

int				cd_alloc_error(void)
{
	ft_eprintf("cd: failed to allocate memory\n");
	return (FUNCT_FAILURE);
}

int				cd_print_usage(void)
{
	ft_eprintf("cd: usage: cd [-L|-P] [dir]\n");
	return (FUNCT_ERROR);
}

int				cd_change_dir_error(char *path)
{
	ft_putstr_fd("cd: ", 2);
	if (path == NULL)
		ft_putendl_fd("could not get current working directory parsing: ", 2);
	else if (access(path, F_OK) == -1)
		ft_putstr_fd("no such file or directory: ", 2);
	else if (access(path, R_OK) == -1)
		ft_putstr_fd("permission denied: ", 2);
	else
		ft_putstr_fd("not a directory: ", 2);
	ft_putendl_fd(path, 2);
	return (FUNCT_ERROR);
}

static void		this_does_some(char *path, char buf[MAXPATHLEN], int *i, int *j)
{
	if (path[*i] != '/')
	{
		if (buf[*j - 1] != '/')
		{
			buf[*j] = '/';
			(*j)++;
		}
		while (path[*i] && path[*i] != '/')
		{
			buf[*j] = path[*i];
			(*j)++;
			(*i)++;
		}
	}
	(*i)++;
}

char			*cd_get_correct_path(char *old_path, char *path)
{
	int		i;
	int		j;
	char	buf[MAXPATHLEN];

	if (*path == '/')
		return (ft_strdup(path));
	ft_strcpy(buf, old_path);
	i = 0;
	j = ft_strlen(buf);
	while (path[i])
	{
		if (path[i] == '.' && path[i + 1] != '\0')
		{
			i++;
			if (path[i] == '.')
			{
				i++;
				while (buf[j] != '/')
					j--;
			}
			continue ;
		}
		this_does_some(path, buf, &i, &j);
	}
	return (ft_strsub(buf, 0, j));
}
