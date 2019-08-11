/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   auto_get_filelst.c                                 :+:    :+:            */
/*                                                     +:+                    */
/*   By: mavan-he <mavan-he@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/08/11 12:28:38 by mavan-he       #+#    #+#                */
/*   Updated: 2019/08/11 12:45:54 by mavan-he      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"
#include <dirent.h>

static int	check_dir(DIR *d, char *match, t_list **matchlst)
{
	struct dirent	*dir;
	int				match_len;

	dir = readdir(d);
	match_len = ft_strlen(match);
	while (dir != NULL)
	{
		if (ft_strnequ(match, dir->d_name, match_len) == true)
		{
			if (auto_add_tolst(matchlst, dir->d_name) == FUNCT_ERROR)
				return (FUNCT_ERROR);
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
		if (check_dir(dir, match, matchlst) == FUNCT_ERROR)
		{
			closedir(dir);
			return (FUNCT_ERROR);
		}
		closedir(dir);
	}
	return (FUNCT_SUCCESS);
}
