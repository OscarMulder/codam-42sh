/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   builtin_cd_error.c                                 :+:    :+:            */
/*                                                     +:+                    */
/*   By: omulder <omulder@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/07/31 17:54:03 by omulder        #+#    #+#                */
/*   Updated: 2019/07/31 17:54:22 by omulder       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"

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
