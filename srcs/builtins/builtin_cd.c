/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   builtin_cd.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: mavan-he <mavan-he@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/04/25 17:17:25 by mavan-he       #+#    #+#                */
/*   Updated: 2019/05/02 10:21:19 by rkuijper      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

/*
** WORK IN PROGRESS
**
** Are we going to handle -L -P -e ??
** -P always overrules -L
**
** usage: cd cd [-L|[-P [-e]] [directory]
**
** TODO:
**	- Handling -L and -P flags when changing dirs. - (Semi done: untested).
**
** DONE:
**	- Handling multiple arguments.
**	- Parsing -L and -P flags correctly.
**	- When HOME env variable is not present, change default home to `/`.
**	- Changing to home dir when no args are given.
**
** - NO ARGS:
** 	Change to HOME
** - DISPLAY DIRECTORY:
** 	If CDPATH is used or - is the first arg. Write path to stdout if chdir was succesful.
** - CD -:
** 	cd to oldpwd
** - MORE THAN ONE ARG:
** 	Any additional arguments following directory are ignored.
** - SHELL VAR CDPATH:
** 	If CDPATH exists, search for directory in CDAPTH (only if directory does not start with a /)
** - FLAG P:
** 	Do not follow symbolic links
** - RETURN:
** 	0 on success, else -1
**
**
** The -P option means to not follow symbolic links:
** symbolic links are resolved while cd is traversing directory and
** before processing an instance of ‘..’ in directory.
**
** By default, or when the -L option is supplied,
** symbolic links in directory are resolved after cd processes an instance of ‘..’ in directory.
**
** If ‘..’ appears in directory,
** it is processed by removing the immediately preceding pathname component,
** back to a slash or the beginning of directory.
**
** If the -e option is supplied with -P and the current working directory
** cannot be successfully determined after a successful directory change,
** cd will return an unsuccessful status.
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
	if (!cwd)
		return (FUNCT_ERROR);
	if (cd_flag & CD_OPT_LP) // Check if this works.
	{
		if (lstat(path, &info))
			return (FUNCT_ERROR);
		if ((info.st_mode & S_IFMT) == S_IFLNK)
		{
			link_buf[readlink(path, buf, MAXPATHLEN)] = '\0';
			path =  link_buf;
		}
	}
	if (!chdir(path))
	{
		if (print)
			ft_putendl(path);
		env_set_value("OLDPWD", cwd, env);
		cwd = getcwd(buf, MAXPATHLEN);
		if (!cwd)
			return (FUNCT_ERROR);
		env_set_value("PWD", cwd, env);
	}
	else
		return (cd_change_dir_error(path));
	return (FUNCT_SUCCESS);
}

/*
** @brief			Parses all -L and -P flags; returns on non-valid flags.
** @param args		Reference to the original builtin_cd args list.
** @param cd_flag	Reference to a character datatype used for option flagging.
*/
static int	cd_parse_flags(char ***args, char *cd_flag)
{
	int i;

	*cd_flag = CD_OPT_LL;
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

/*
** @brief		Builtin cd functionality with flags and extern environment variables.
** @param args 	Arguments for the builtin-cd functionality (split command).
** @param env	Environment/extern variables of the current shell instance.
** @return		Returns FUNCT_SUCCESS or FUNCT_ERROR depending on process.
*/
int			builtin_cd(char **args, char **env)
{
	int		result;
	char	cd_flag;
	char	*home;

	args++; // Get rid of the preliminary cd arg, we don't need that.
	home = env_get_value("HOME=", env);
	if (!home)
		home = "/";

	// The flag parse loop stops either when an invalid option is encountered, or when there are no more parameters starting with '-'.
	if (!cd_parse_flags(&args, &cd_flag))
		return (FUNCT_ERROR);

	// No multiple arguments need to be parsed!
	// First argument after flag parsing is used as a path to cd.

	// cd home dir if no additional arguments.
	if (!args[0])
		return (cd_change_dir(home, env, cd_flag, 0));	
	if (ft_strequ(args[0], "--")) // cd home dir.
		return (cd_change_dir(home, env, cd_flag, 0));
	if (args[0][0] == '-' && !args[0][1]) // cd last accessed directory.
		return (cd_change_dir(env_get_value("OLDPWD=", env), env, cd_flag, 1));
		
	// If all 'special' cd tests fail, try to cd the given argument.
	return (cd_change_dir(args[0], env, cd_flag, 0));
}
