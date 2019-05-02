/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   builtin_cd.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: mavan-he <mavan-he@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/04/25 17:17:25 by mavan-he       #+#    #+#                */
/*   Updated: 2019/05/02 14:01:36 by mavan-he      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

/*
** cd: usage: cd [-L|-P] [dir]
**
** TODO:
**	- Handling -L and -P flags when changing dirs. - (Semi done: untested).
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
**
*/

#include "vsh.h"
#include <sys/stat.h>
#include <sys/param.h>

static int		cd_change_dir_error(char *path)
{
	ft_putstr_fd("cd: ", 2);
	if (access(path, F_OK) == -1)
		ft_putstr_fd("no such file or directory: ", 2);
	else if (access(path, R_OK) == -1)
		ft_putstr_fd("permission denied: ", 2);
	else
		ft_putstr_fd("not a directory: ", 2);
	ft_putendl_fd(path, 2);
	return (FUNCT_ERROR);
}

static int		cd_change_dir(char *path, char **env, char cd_flag, int print)
{
	struct stat	info;
	char		*cwd;
	char		buf[MAXPATHLEN];
	char		link_buf[MAXPATHLEN];

	cwd = getcwd(buf, MAXPATHLEN);
	if (!cwd) // WHAT ERROR?
		return (FUNCT_ERROR);
	if (cd_flag & CD_OPT_LP) // Check if this works.
	{
		if (!lstat(path, &info) && (info.st_mode & S_IFMT) == S_IFLNK)
		{
			link_buf[readlink(path, buf, MAXPATHLEN)] = '\0';
			path =  link_buf;
		}
	}
	if (!chdir(path))
	{
		if (print)
			ft_putendl(path);
		var_set_value("OLDPWD", cwd, env);
		cwd = getcwd(buf, MAXPATHLEN);
		if (!cwd) // WHAT ERROR?
			return (FUNCT_ERROR);
		var_set_value("PWD", cwd, env);
	}
	else
		return (cd_change_dir_error(path));
	return (FUNCT_SUCCESS);
}

static int	cd_parse_flags(char ***args, char *cd_flag)
{
	int i;

	// PROCESS SPACES CORRECTLY!

	while ((*args)[0] && (*args)[0][0] == '-')
	{
		i = 1;
		if ((*args)[0][0] == '-' && !(*args)[0][1] ||
			ft_strequ((*args)[0], "--"))
			return (FUNCT_SUCCESS);
		while ((*args)[0][i])
		{
			if ((*args)[0][i] == 'P')
				(*cd_flag) = CD_OPT_LP;
			else if ((*args)[0][i] == 'L')
				(*cd_flag) = CD_OPT_LL;
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

int			builtin_cd(char **args, char **env)
{
	int		result;
	char	cd_flag;
	char	*home;

	args++;
	cd_flag = CD_OPT_LL;
	home = var_get_value("HOME=", env);
	if (!cd_parse_flags(&args, &cd_flag))
		return (FUNCT_ERROR);
	if (!args[0] || ft_strequ(args[0], "--"))
	{
		if (!home)
		{
			ft_putendl_fd("42sh: cd: HOME not set", 2);
			return (FUNCT_ERROR);
		}
		return (cd_change_dir(home, env, cd_flag, 0));
	}
	if (args[0][0] == '-' && !args[0][1])
		return (cd_change_dir(var_get_value("OLDPWD=", env), env, cd_flag, 1));
	return (cd_change_dir(args[0], env, cd_flag, 0));
}
