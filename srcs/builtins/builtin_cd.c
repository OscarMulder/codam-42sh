/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   builtin_cd.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: mavan-he <mavan-he@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/04/25 17:17:25 by mavan-he       #+#    #+#                */
/*   Updated: 2019/05/01 15:16:59 by rkuijper      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

/*
** WORK IN PROGRESS
**
** Are we going to handle -L -P -e ??
** -P always overrules -L
** Arguments like `cd -P -L -P -L -P -L path/to/dir` are valid.
** Arguments like `cd -LPPLLLPPLLLLLPPL path/to/dir` are also valid.
** Arguments like `cd -P -L path/to/dir -P -L` are not.
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
		if (!lstat(path, &info))
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
** @brief			Parses all -L and -P flags; returns on other non-valid flags.
** @param args		Reference to the original builtin_cd args list.
** @param cd_flag	Reference to a character datatype used for option flagging.
*/
static void	cd_parse_flags(char ***args, char *cd_flag)
{
	int i;

	while ((*args)[0] && (*args)[0][0] == '-')
	{
		i = 1;
		while ((*args)[0][i])
		{
			if ((*args)[0][i] == 'P')
				(*cd_flag) |= CD_OPT_LP;
			else if ((*args)[0][i] == 'L')
				(*cd_flag) |= CD_OPT_LL;
			else
				return ;
			i++;
		}
		(*args)++;
	}
}

static int	cd_multi_arg(char **args, char **environ, char cd_flag)
{
	int		result;
	char	*cwd;
	char	*tmp;
	char	buf[MAXPATHLEN];

	// If there's no second parameter.
	if (!args[1])
		return (FUNCT_FAILURE);

	// If there's too many arguments.
	if (args[2])
	{
		ft_putendl_fd("cd: too many arguments", 2);
		return (FUNCT_ERROR);
	}

	cwd = getcwd(buf, MAXPATHLEN);
	// if (!cwd)
	tmp = ft_strreplace(cwd, args[0], args[1]);

	// If tmp was not created or no string substitution took place.
	if (!tmp)
	{
		ft_dprintf(2, "cd: string not in pwd: %s\n", args[0]);
		free(tmp);
		return (FUNCT_ERROR);
	}

	// Change directories with the newly created string.
	result = cd_change_dir(tmp, environ, cd_flag, 1);
	free(tmp);
	return (result);
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

	// Parse cd flags. -L flag on by default.
	cd_flag = 0 | CD_OPT_LL;
	cd_parse_flags(&args, &cd_flag);

	// cd home dir if no additional arguments.
	if (!args[0])
		return (cd_change_dir(home, env, cd_flag, 0));
		
	// Parse two cd arguments if there are any.
	result = cd_multi_arg(args, env, cd_flag);
	if (result)
		return (result);
	
	if (ft_strequ(args[0], "--")) // cd home dir.
		return (cd_change_dir(home, env, cd_flag, 0));
	else if (args[0][0] == '-' && !args[0][1]) // cd last accessed directory.
		return (cd_change_dir(env_get_value("OLDPWD=", env), env, cd_flag, 1));
		
	// If all 'special' cd tests fail, try to cd the given argument.
	return (cd_change_dir(args[0], env, cd_flag, 0));
}
