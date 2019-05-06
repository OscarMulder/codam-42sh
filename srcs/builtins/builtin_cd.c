/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   builtin_cd.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: mavan-he <mavan-he@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/04/25 17:17:25 by mavan-he       #+#    #+#                */
/*   Updated: 2019/05/06 14:37:04 by rkuijper      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

/*
** cd: usage: cd [-L|-P] [dir]
**
** TODO:
**	- Handling -L and -P flags when changing dirs.
**	- Parsing -L and -P flags correctly.
**
** FUNCTIONALITY:
** - NO ARGS:
** 	cd to HOME
** - CD -:
** 	cd to OLDPWD
** - CD --:
** 	cd to HOME
** - DISPLAY DIRECTORY:
**	Only when cd -: Write path to stdout if chdir was succesful.
** - MORE THAN ONE DIR:
** 	Any additional arguments following directory are ignored.
** - FLAG P:
** 	Do not follow symbolic links
** - RETURN:
** 	0 on success, else -1
**
** ERRORS
** - When OLDPWD is not set
** - When HOME is not set
** - When cwd doesn't return correctly.
**
*/

#include "vsh.h"
#include <sys/stat.h>
#include <sys/param.h>

static int		cd_change_dir_error(char *path)
{
	ft_putstr_fd("cd: ", 2);
	if (path == NULL)
	{
		ft_putendl_fd("could not get current working directory", 2);
		return (FUNCT_ERROR);
	}
	if (access(path, F_OK) == -1)
		ft_putstr_fd("no such file or directory: ", 2);
	else if (access(path, R_OK) == -1)
		ft_putstr_fd("permission denied: ", 2);
	else
		ft_putstr_fd("not a directory: ", 2);
	ft_putendl_fd(path, 2);
	return (FUNCT_ERROR);
}

static char		*cd_get_correct_path(char *old_path, char *path)
{
	int i;
	int j;	
	char *tmp_path;
	char *tmp_path2;

	if (*path == '/') // Direct/absolute reference.
		return (ft_strdup(path));

	tmp_path = ft_strdup(old_path);
	
	i = 0;
	j = ft_strlen(old_path);
	if (old_path[j] == '/')
		j--;
	while (path[i])
	{
		// Stay in the current directory.
		if (path[i] == '.' && (path[i + 1] == '\0' || path[i + i] == '/'))
		{
			i++;
			if (path[i] == '/')
				i++;
			continue ;
		}
		// Move up one directory.
		if (ft_strncmp(&path[i], "..\0", 3) == 0 || ft_strncmp(&path[i], "../", 3) == 0)
		{
			// Move the begin index (j) back to the previous '/'.
			while (old_path[j] != '/')
				j--;
			j--;
			i += 2;
			if (path[i] == '/')
				i++;
			continue;
		}

		if (path[i] != '/')
		{
			// Append the filename in the current 'path' index to the new path.
			tmp_path2 = ft_strsub(tmp_path, 0, j);
			if (tmp_path2[ft_strlen(tmp_path2)] != '/')
				tmp_path2 = ft_strjoinchrfree(tmp_path2, '/', 1);
			
			while (path[i] && path[i] != '/')
			{
				tmp_path2 = ft_strjoinchrfree(tmp_path2, path[i], 1);
				i++;
			}
			free(tmp_path);
			tmp_path = tmp_path2;
		}
		if (!path[i])
			break ;
		i++;
	}
	return (tmp_path);
}

static void		cd_post_process_var(char *old_path, char *path, char **env, char cd_flag)
{
	char *correct_path;

	correct_path = cd_get_correct_path(old_path, path);
	var_set_value("OLDPWD", var_get_value("PWD", env), env);
	if (cd_flag == CD_OPT_PU)
		var_set_value("PWD", getcwd(NULL, 0), env);
	else
		var_set_value("PWD", correct_path, env);
	free(correct_path);
}

static int		cd_change_dir(char *path, char ***env, char cd_flag, int print)
{
	char		*old_path;
	
	if (cd_flag == CD_OPT_PU)
		old_path = getcwd(NULL, 0);
	else
		old_path = ft_strdup(var_get_value("PWD", *env));
	if (old_path == NULL)
		return (cd_change_dir_error(NULL));
	if (chdir(path) != 0)
		return (cd_change_dir_error(path));
	if (print)
		ft_putendl(path);
	cd_post_process_var(old_path, path, *env, cd_flag);
	free(old_path);
	return (FUNCT_SUCCESS);
}

static int	cd_parse_flags(char ***args, char *cd_flag)
{
	int i;

	while ((*args)[0] != NULL && (*args)[0][0] == '-')
	{
		if ((*args)[0][1] == 0 || ft_strequ((*args)[0], "--"))
			return (FUNCT_SUCCESS);
		i = 1;
		while ((*args)[0][i] != 0)
		{
			if ((*args)[0][i] == 'P')
				(*cd_flag) = CD_OPT_PU;
			else if ((*args)[0][i] == 'L')
				(*cd_flag) = CD_OPT_LU;
			else
			{
				ft_dprintf(2, "minishell: cd: -%c: invalid option\n\
				cd: usage: cd [-L|-P] [dir]\n", (*args)[0][i]);
				return (FUNCT_FAILURE);
			}
			i++;
		}
		(*args)++;
	}
	return (FUNCT_SUCCESS);
}

static int	cd_parse_dash(char *path, char ***env, char cd_flag, char *var)
{
	if (path == NULL)
	{
		ft_dprintf(2, "42sh: cd: %s: not set\n", var);
		return (FUNCT_ERROR);
	}
	if (ft_strequ(var, "HOME"))
		return (cd_change_dir(path, env, cd_flag, 0));
	else
		return (cd_change_dir(path, env, cd_flag, 1));
}

int			builtin_cd(char **args, char ***env)
{
	char	cd_flag;
	char	*path;

	args++;
	cd_flag = CD_OPT_LU;
	if (cd_parse_flags(&args, &cd_flag) == 0)
		return (FUNCT_ERROR);
	if (args[0] == NULL || ft_strequ(args[0], "--"))
	{
		path = var_get_value("HOME", *env);
		return (cd_parse_dash(path, env, cd_flag, "HOME"));
	}
	if (args[0][0] == '-' && args[0][1] == 0)
	{
		path = var_get_value("OLDPWD", *env);
		return (cd_parse_dash(path, env, cd_flag, "OLDPWD"));
	}
	return (cd_change_dir(args[0], env, cd_flag, 0));
}
