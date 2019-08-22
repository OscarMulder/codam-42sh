/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_islink.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: mavan-he <mavan-he@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/08/04 12:51:00 by mavan-he       #+#    #+#                */
/*   Updated: 2019/08/22 11:54:21 by tde-jong      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <sys/stat.h>

int		ft_islink(const char *path)
{
	struct stat	stat_path;

	if (lstat(path, &stat_path) == -1)
		return (-1);
	return (S_ISLNK(stat_path.st_mode));
}
