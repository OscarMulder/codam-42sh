/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   auto_get_filelst.c                                 :+:    :+:            */
/*                                                     +:+                    */
/*   By: mavan-he <mavan-he@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/08/11 12:28:38 by mavan-he       #+#    #+#                */
/*   Updated: 2019/08/15 12:42:35 by omulder       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"
#include <dirent.h>

/*
** For now we are only using the lstat, not sure if this is the correct one
** but it seems to work
*/

static int	is_dir(char *path, int l)
{
	struct stat buf;

	if (l)
	{
		if (lstat(path, &buf) == -1)
			return (0);
	}
	else
	{
		if (stat(path, &buf) == -1)
			return (0);
	}
	return (S_ISDIR(buf.st_mode));
}

static int	check_dir(DIR *d, char *match, t_list **matchlst, char *path)
{
	struct dirent	*dir;
	int				match_len;

	dir = readdir(d);
	match_len = ft_strlen(match);
	while (dir != NULL)
	{
		if (ft_strnequ(match, dir->d_name, match_len) == true)
		{
			if (!(ft_strequ(match, "") == true &&
			(ft_strequ(dir->d_name, ".") == true ||
			ft_strequ(dir->d_name, "..") == true)))
			{
				if (is_dir(ft_strjoinfree_s1(ft_strjoin(path, "/"), dir->d_name), true)) // LEAKS THE SHIT
					if (auto_add_tolst(matchlst, ft_strjoin(dir->d_name, "/")) == FUNCT_ERROR) // LEAKS
						return (FUNCT_ERROR);
				else
					if (auto_add_tolst(matchlst, dir->d_name) == FUNCT_ERROR)
						return (FUNCT_ERROR);
			}
		}
		dir = readdir(d);
	}
	return (FUNCT_SUCCESS);
}

int			auto_get_filelst(char *match, char *path, t_list **matchlst)
{
	DIR				*dir;

	dir = opendir(path);
	if (dir != NULL)
	{
		if (check_dir(dir, match, matchlst, path) == FUNCT_ERROR)
		{
			closedir(dir);
			return (FUNCT_ERROR);
		}
		closedir(dir);
	}
	return (FUNCT_SUCCESS);
}
