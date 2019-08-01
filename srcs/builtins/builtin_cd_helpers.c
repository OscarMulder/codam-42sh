/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   builtin_cd_helpers.c                               :+:    :+:            */
/*                                                     +:+                    */
/*   By: omulder <omulder@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/07/30 16:46:21 by omulder        #+#    #+#                */
/*   Updated: 2019/07/31 17:54:17 by omulder       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"
#include <sys/param.h>

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

static void		do_something(char *path, char buf[MAXPATHLEN], int *i, int *j)
{
	(*i)++;
	if (path[*i] == '.')
	{
		(*i)++;
		while (buf[*j] != '/')
			(*j)--;
	}
}

char			*cd_get_correct_path(char *old_path, char *path)
{
	int		i;
	int		j;
	char	buf[MAXPATHLEN];

	ft_bzero(buf, MAXPATHLEN);
	if (*path == '/')
		return (ft_strdup(path));
	ft_strcpy(buf, old_path);
	i = 0;
	j = ft_strlen(buf);
	while (path[i])
	{
		if (path[i] == '.' && path[i + 1] != '\0')
		{
			do_something(path, buf, &i, &j);
			continue ;
		}
		this_does_some(path, buf, &i, &j);
	}
	return (ft_strsub(buf, 0, j));
}
