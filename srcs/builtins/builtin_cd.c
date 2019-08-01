/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   builtin_cd.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: omulder <omulder@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/07/30 12:41:21 by omulder        #+#    #+#                */
/*   Updated: 2019/08/01 10:32:12 by omulder       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"
#include <sys/stat.h>
#include <errno.h>
#include <string.h>

/*
**  cd: usage: cd [-L|-P] [dir]
**  TODO:
**	- Handling -L and -P flags when changing dirs.
**	- Parsing -L and -P flags correctly.
**  FUNCTIONALITY:
**  - NO ARGS:
**  	cd to HOME
**  - CD -:
**  	cd to OLDPWD
**  - CD --:
**  	cd to HOME
**  - DISPLAY DIRECTORY:
** 	Only when cd -: Write path to stdout if chdir was succesful.
**  - MORE THAN ONE DIR:
**  	Any additional arguments following directory are ignored.
**  - FLAG P:
**  	Do not follow symbolic links (parse physical address).
**  - RETURN:
**  	0 on success, else -1
**  ERRORS
**  - When OLDPWD is not set
**  - When HOME is not set
**  - When cwd doesn't return correctly.
*/

char		*ms_make_path(char *dir, char *file)
{
	char *temp;
	char *path;

	if (!file || !dir)
		return (NULL);
	if (file[0] == '/')
		return (ft_strdup(file));
	temp = ft_strjoin(dir, "/");
	path = ft_strjoin(temp, file);
	ft_strdel(&temp);
	return (path);
}

static int		cd_post_process_var(char *old_path, char *path,
t_envlst *envlst, char cd_flag)
{
	char		*correct_path;
	struct stat	ptr;
	int			ret;


	ret = lstat(ms_make_path(old_path, path), &ptr);
	ft_printf("path: %s - ret: %d - errno: %s\n", path, ret, strerror(errno));
	if (S_ISLNK(ptr.st_mode) && cd_flag == BUILTIN_CD_LU)
		correct_path = ms_make_path(old_path, path);
	else
		correct_path = getcwd(NULL, 0);
	if (correct_path == NULL)
		return (cd_alloc_error());
	if (env_add_extern_value(envlst, "OLDPWD", old_path) == FUNCT_ERROR)
		return (cd_alloc_error());
	if (cd_flag == BUILTIN_CD_PU)
	{
		ft_strdel(&correct_path);
		correct_path = getcwd(NULL, 0);
		if (correct_path == NULL)
			return (cd_alloc_error());
	}
	if (env_add_extern_value(envlst, "PWD", correct_path) == FUNCT_ERROR)
		return (cd_alloc_error());
	ft_strdel(&correct_path);
	return (FUNCT_SUCCESS);
}

static int		cd_change_dir(char *path, t_envlst *envlst, char cd_flag,
int print)
{
	char		*pwd;
	char		*old_path;

	pwd = env_getvalue("PWD", envlst);
	if (cd_flag == BUILTIN_CD_LU && pwd != NULL)
		old_path = ft_strdup(pwd);
	else
		old_path = getcwd(NULL, 0);
	if (old_path == NULL)
		return (cd_change_dir_error(NULL));
	if (chdir(path) != 0)
		return (cd_change_dir_error(path));
	if (print)
		ft_putendl(path);
	if (cd_post_process_var(old_path, path, envlst, cd_flag) == FUNCT_ERROR)
		return (FUNCT_ERROR);
	free(old_path);
	return (FUNCT_SUCCESS);
}

static int		cd_parse_flags(char **args, char *cd_flag, int *countflags)
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
				*cd_flag = BUILTIN_CD_PU;
			else if (args[j][i] == 'L')
				*cd_flag = BUILTIN_CD_LU;
			else
				return (cd_invalid_option(args[j][i]));
			i++;
		}
		j++;
	}
	*countflags = j - 1;
	return (FUNCT_SUCCESS);
}

static int		cd_parse_dash(char *path, t_envlst *envlst, char cd_flag,
char *var)
{
	if (path == NULL || path[0] == '\0')
	{
		ft_dprintf(2, "vsh: cd: %s: not set\n", var);
		return (FUNCT_ERROR);
	}
	if (ft_strequ(var, "HOME"))
		return (cd_change_dir(path, envlst, cd_flag, 0));
	else
		return (cd_change_dir(path, envlst, cd_flag, 1));
}

int				builtin_cd(char **args, t_envlst *envlst)
{
	char	cd_flag;
	char	*path;
	int		flags;

	cd_flag = BUILTIN_CD_LU;
	flags = 0;
	if (cd_parse_flags(args, &cd_flag, &flags) == 0)
		return (FUNCT_ERROR);
	if (args[1 + flags] == NULL || ft_strequ(args[1 + flags], "--"))
	{
		path = env_getvalue("HOME", envlst);
		return (cd_parse_dash(path, envlst, cd_flag, "HOME"));
	}
	if (args[1 + flags][0] == '-' && args[1 + flags][1] == '\0')
	{
		path = env_getvalue("OLDPWD", envlst);
		return (cd_parse_dash(path, envlst, cd_flag, "OLDPWD"));
	}
	if (args[1 + flags][0] == '.' && args[1 + flags][1] == '\0')
	{
		path = env_getvalue("PWD", envlst);
		if (path == NULL)
			return (cd_alloc_error());
		return (cd_change_dir(path, envlst, cd_flag, 0));
	}
	return (cd_change_dir(args[1 + flags], envlst, cd_flag, 0));
}
