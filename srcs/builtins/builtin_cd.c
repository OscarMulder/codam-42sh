/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   builtin_cd.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: omulder <omulder@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/07/30 12:41:21 by omulder        #+#    #+#                */
/*   Updated: 2019/07/30 16:23:46 by omulder       ########   odam.nl         */
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
** 	Do not follow symbolic links (parse physical address).
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

static char		*cd_get_correct_path(char *old_path, char *path)
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
		if (path[i] == '.' &&
			(path[i + 1] != '\0' || path[i + 1] == '.' || path[i + 1] == '/'))
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
		if (path[i] != '/')
		{
			if (buf[j - 1] != '/')
				buf[j++] = '/';
			while (path[i] && path[i] != '/')
			{
				buf[j++] = path[i];
				i++;
			}
		}
		if (path[i])
			i++;
	}
	return (ft_strsub(buf, 0, j));
}

static void		cd_post_process_var(char *old_path, char *path, t_envlst *envlst, char cd_flag)
{
	char *correct_path;

	correct_path = cd_get_correct_path(old_path, path);
	env_add_extern_value(envlst, "OLDPWD", old_path);
	if (cd_flag == BUILTIN_CD_PU)
	{
		free(correct_path);
		correct_path = getcwd(NULL, 0);
		env_add_extern_value(envlst, "PWD", correct_path);
	}
	else
		env_add_extern_value(envlst, "PWD", correct_path);
	free(correct_path);
}

static int		cd_change_dir(char *path, t_envlst *envlst, char cd_flag, int print)
{
	char		*pwd;
	char		*old_path;
	
	pwd = env_getvalue("PWD", envlst);
	if (cd_flag == BUILTIN_CD_LU && pwd)
		old_path = ft_strdup(pwd);
	else
		old_path = getcwd(NULL, 0);
	if (old_path == NULL)
		return (cd_change_dir_error(NULL));
	if (chdir(path) != 0)
		return (cd_change_dir_error(path));
	if (print)
		ft_putendl(path);
	cd_post_process_var(old_path, path, envlst, cd_flag);
	free(old_path);
	return (FUNCT_SUCCESS);
}

static int	cd_parse_flags(char **args, char *cd_flag)
{
	int i;
	int j;

	j = 1;
	while (args[j] != NULL && args[j][0] == '-')
	{
		if (args[j][1] == '\0' || ft_strequ(args[j], "--"))
			return (FUNCT_SUCCESS);
		i = 1;
		while (args[j][i] != 0)
		{
			if (args[j][i] == 'P')
				(*cd_flag) = BUILTIN_CD_PU;
			else if (args[j][i] == 'L')
				(*cd_flag) = BUILTIN_CD_LU;
			else
			{
				ft_dprintf(2, "vsh: cd: -%c: invalid option\n\
				cd: usage: cd [-L|-P] [dir]\n", args[j][i]);
				return (FUNCT_ERROR);
			}
			i++;
		}
		j++;
	}
	return (FUNCT_SUCCESS);
}

static int	cd_parse_dash(char *path, t_envlst *envlst, char cd_flag, char *var)
{
	if (path == NULL)
	{
		ft_dprintf(2, "vsh: cd: %s: not set\n", var);
		return (FUNCT_ERROR);
	}
	if (ft_strequ(var, "HOME"))
		return (cd_change_dir(path, envlst, cd_flag, 0));
	else
		return (cd_change_dir(path, envlst, cd_flag, 1));
}

int			builtin_cd(char **args, t_envlst *envlst)
{
	char	cd_flag;
	char	*path;

	cd_flag = BUILTIN_CD_LU;
	if (cd_parse_flags(args, &cd_flag) == 0)
		return (FUNCT_ERROR);
	if (args[1] == NULL || ft_strequ(args[1], "--"))
	{
		path = env_getvalue("HOME", envlst);
		return (cd_parse_dash(path, envlst, cd_flag, "HOME"));
	}
	if (args[1][0] == '-' && args[1][1] == '\0')
	{
		path = env_getvalue("OLDPWD", envlst);
		return (cd_parse_dash(path, envlst, cd_flag, "OLDPWD"));
	}
	return (cd_change_dir(args[1], envlst, cd_flag, 0));
}
