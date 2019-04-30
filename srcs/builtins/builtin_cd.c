/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   builtin_cd.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: mavan-he <mavan-he@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/04/25 17:17:25 by mavan-he       #+#    #+#                */
/*   Updated: 2019/04/30 12:44:43 by mavan-he      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

/*
** WORK IN PROGRESS
**
** Are we going to handle -L -P -e ??
**
** usage: cd cd [-L|[-P [-e]] [directory]
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
#include <sys/param.h>

static int		cd_change_dir(char *path, int p)
{
	char *cwd;
	char buf[MAXPATHLEN];

	cwd = getcwd(buf, MAXPATHLEN);
	// if (cwd == NULL)
	// 	return (error_funct());
	if (chdir(path) == 0)
	{
		if (p)
			ft_putendl(path);
		env_setkeyval("OLDPWD", cwd);
		cwd = getcwd(buf, MAXPATHLEN);
	// if (cwd == NULL)
	// 	return (error_funct());
		env_setkeyval("PWD", cwd);
	}
	else
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
	return (FUNCT_SUCCESS);
}


int			builtin_cd(char **args, char **env)
{
	int		res;
	char	*home;

	res = 0;
	home = env_getval("HOME=");
	if (!args[0])
		return (cd_change_dir(home, 0));
	res = cd_two_args(args);
	if (res)
		return (res - 1);
	if (ft_strequ(args[0], "--"))
		return (cd_change_dir(home, 0));
	else if (args[0][0] == '-' && !args[0][2])
		return (cd_change_dir(env_getval("OLDPWD="), 1));
	return (cd_change_dir(args[0], 0));
}







int			builtin_cd(char **args, char **env)
{
	(void)env;
	if (!args[1])
		// Cd home dir.
	// Check two args.
	if (ft_strequ(args[1], "--")) ;
		// Cd home dir.
	else if (ft_strequ(args[1], "-")) ;
		// Cd OLDPWD dir.
		// If OLDPWD is not set (first cd command), display error.
	// Cd the args[1] dir.
	return (FUNCT_SUCCESS);
}

/* void	vsh_cd_replace(char **command_row)
{
	char	*cwd;
	char	*new_path;

	cwd = ft_strnew(MAXPATHLEN);
	getcwd(cwd, MAXPATHLEN);
	new_path = ft_strstrrepl(cwd, command_row[1], command_row[2]);
	ft_strdel(&cwd);
	ft_strdel(&command_row[2]);
	command_row[2] = NULL;
	ft_strdel(&command_row[1]);
	command_row[1] = new_path;
}

void	vsh_cd_replacecwd(char **command_row, char ***vshenviron)
{
	char	*pwd;
	char	*oldpwd;

	oldpwd = ft_strnew(MAXPATHLEN);
	oldpwd = getcwd(oldpwd, MAXPATHLEN);
	vsh_cd_replace(command_row);
	if (!command_row[1])
		vsh_error(E_CDSTRNOT, command_row[0]);
	else if (chdir(command_row[1]) == -1)
	{
		ft_printf("dir: %s\n", command_row[1]);
		vsh_error(vsh_finderror_cd(command_row), command_row[0]);
	}
	else
	{
		pwd = ft_strnew(MAXPATHLEN);
		pwd = getcwd(pwd, MAXPATHLEN);
		vsh_setenv_add("OLDPWD", oldpwd, vshenviron);
		vsh_setenv_add("PWD", pwd, vshenviron);
		ft_strdel(&pwd);
	}
	ft_strdel(&oldpwd);
}

void	vsh_cd_regular(char **command_row, char ***vshenviron)
{
	char	*pwd;
	char	*oldpwd;

	oldpwd = ft_strnew(MAXPATHLEN);
	oldpwd = getcwd(oldpwd, MAXPATHLEN);
	if (!ft_strcmp(command_row[1], "-"))
	{
		ft_strdel(&command_row[1]);
		command_row[1] = vsh_param_to_env("OLDPWD", *vshenviron);
		ft_printf("%s\n", command_row[1]);
	}
	if (chdir(command_row[1]) == -1)
		vsh_error(vsh_finderror_cd(command_row), command_row[0]);
	else
	{
		pwd = ft_strnew(MAXPATHLEN);
		pwd = getcwd(pwd, MAXPATHLEN);
		vsh_setenv_add("OLDPWD", oldpwd, vshenviron);
		vsh_setenv_add("PWD", pwd, vshenviron);
		ft_strdel(&pwd);
	}
	ft_strdel(&oldpwd);
}

void	vsh_cd_noargs(char **command_row, char ***vshenviron,\
int env_path_index)
{
	char		*home;
	char		*pwd;
	char		*oldpwd;

	if (env_path_index != -1)
	{
		oldpwd = ft_strnew(MAXPATHLEN);
		oldpwd = getcwd(oldpwd, MAXPATHLEN);
		home = ft_strdup(&((*vshenviron)[env_path_index][5]));
		if (chdir(home) == -1)
			vsh_error(vsh_finderror_cd(command_row), command_row[0]);
		else
		{
			pwd = ft_strnew(MAXPATHLEN);
			pwd = getcwd(pwd, MAXPATHLEN);
			vsh_setenv_add("OLDPWD", oldpwd, vshenviron);
			vsh_setenv_add("PWD", pwd, vshenviron);
			ft_strdel(&pwd);
		}
		ft_strdel(&oldpwd);
		ft_strdel(&home);
	}
	else if (env_path_index == -1)
		vsh_error(E_CDHOME, command_row[0]);
}

void	vsh_cd(char **command_row, char ***vshenviron)
{
	int			env_path_index;
	int			argc;

	argc = vsh_count_args(command_row);
	env_path_index = vsh_findvar("HOME", *vshenviron);
	if (argc == 1)
		vsh_cd_noargs(command_row, vshenviron, env_path_index);
	else if (argc == 2)
		vsh_cd_regular(command_row, vshenviron);
	else if (argc == 3)
		vsh_cd_replacecwd(command_row, vshenviron);
	else
		vsh_error(E_TOOMNYARGS, command_row[0]);
} */












/*
include <stdlib.h>
#include <unistd.h>
#include "minishell.h"

static int		cd_change_dir(char *path, int p)
{
	char *cwd;
	char buf[4097];

	cwd = getcwd(buf, 4096);
	if (!chdir(path))
	{
		if (p)
			ft_putendl(path);
		env_setkeyval("OLDPWD", cwd);
		cwd = getcwd(buf, 4096);
		env_setkeyval("PWD", cwd);
	}
	else
	{
		ft_putstr_fd("cd: ", 2);
		if (access(path, F_OK) == -1)
			ft_putstr_fd("no such file or directory: ", 2);
		else if (access(path, R_OK) == -1)
			ft_putstr_fd("permission denied: ", 2);
		else
			ft_putstr_fd("not a directory: ", 2);
		ft_putendl_fd(path, 2);
		return (0);
	}
	return (1);
}

static int		cd_two_args(char **args)
{
	int		res;
	char	*cwd;
	char	*tmp;
	char	buf[4096];

	if (!args[1])
		return (0);
	if (args[2])
	{
		ft_putendl_fd("cd: too many arguments", 2);
		return (1);
	}
	cwd = getcwd(buf, 4096);
	tmp = ft_strreplace(cwd, args[0], args[1]);
	if (!tmp)
	{
		ft_putstr_fd("cd: string not in pwd: ", 2);
		ft_putendl_fd(args[0], 2);
		free(tmp);
		return (1);
	}
	res = cd_change_dir(tmp, 1);
	free(tmp);
	return (res + 1);
}

int				builtin_cd(char **args)
{
	int		res;
	char	*home;

	res = 0;
	home = env_getval("HOME=");
	if (!args[0])
		return (cd_change_dir(home, 0));
	res = cd_two_args(args);
	if (res)
		return (res - 1);
	if (ft_strequ(args[0], "--"))
		return (cd_change_dir(home, 0));
	else if (args[0][0] == '-' && !args[0][2])
		return (cd_change_dir(env_getval("OLDPWD="), 1));
	return (cd_change_dir(args[0], 0));
}
 */