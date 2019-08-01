/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   builtin_cd.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: omulder <omulder@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/07/30 12:41:21 by omulder        #+#    #+#                */
/*   Updated: 2019/08/01 15:08:35 by jbrinksm      ########   odam.nl         */
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

int		cd_stayhere(char **newpath, char *argpath)
{
	int i;

	i = ft_strlen(*newpath);
	if (i > 0)
		i--;
	/* If we're at the end of argpath and thus we don't need a '/' at the end */
	if ((*newpath)[i] == '/' && i != 0 && (argpath[1] == '\0'
		|| (argpath[2] == '/' && argpath[2] == '\0')))
		(*newpath)[i] = '\0';
	if (argpath[1] == '/')
		return (2);
	return (1);
}

int		cd_gobackone(char **newpath, char *argpath)
{
	int i;

	i = ft_strlen(*newpath);
	if (i > 0)
		i--;
	while ((*newpath)[i] != '/' && i > 0)
	{
		(*newpath)[i] = '\0';
		i--;
	}
	/* If we're at the end of argpath and thus we don't need a '/' at the end */
	if ((*newpath)[i] == '/' && i != 0 && (argpath[2] == '\0'
		|| (argpath[2] == '/' && argpath[3] == '\0')))
		(*newpath)[i] = '\0';
	if (argpath[2] == '/')
		return (3);
	return (2);
}

int		cd_addsymdir(char **newpath, char *argpath)
{
	int i;
	int	arg_i;

	if (*argpath == '/')
	{
		ft_putendl("NEED TO ADD A HANDLER FOR THIS SITUATION");
		exit(1);
	}

	i = ft_strlen(*newpath);
	if (i != 0 && (*newpath)[i - 1] != '/')
	{
		(*newpath)[i] = '/';
		i++;
	}
	arg_i = 0;
	while (argpath[arg_i] != '/' && argpath[arg_i] != '\0')
	{
		(*newpath)[i] = argpath[arg_i];
		i++;
		arg_i++;
	}
	if (argpath[arg_i] == '/' && argpath[arg_i + 1] != '\0')
	{
		(*newpath)[i] = argpath[arg_i];
		i++;
		arg_i++;
	}
	(*newpath)[i] = '\0';
	return (arg_i + 1);
}

char		*cd_make_new_sympath(char *currpath, char *argpath)
{
	char	*newpath;
	int		i;

	if (currpath == NULL || argpath == NULL)
		return (NULL);
	ft_printf(">:%s\n>:%s\n", currpath, argpath);
	i = 0;
	newpath = ft_strnew(ft_strlen(currpath) + ft_strlen(argpath));
	if (newpath == NULL)
		return (NULL);
	if (*argpath == '/') //INSERT FUNCTION
		return (NULL); //dosomethingelse
	ft_strcpy(newpath, currpath);
	while (argpath[i] != '\0')
	{
		if (ft_strequ(&argpath[i], ".") || ft_strnequ(&argpath[i], "./", 2))
			i += cd_stayhere(&newpath, &argpath[i]);
		else if (ft_strequ(&argpath[i], "..") || ft_strnequ(&argpath[i], "../", 3))
			i += cd_gobackone(&newpath, &argpath[i]);
		else
			i += cd_addsymdir(&newpath, &argpath[i]);
		ft_printf("i IS>%i\n", i);
	}
	ft_printf(">>>%s\n", newpath);
	return (newpath);
}

static int		cd_post_process_var(char *currpath, char *newpath,
t_envlst *envlst, char cd_flag)
{
	if (newpath == NULL)
		return (cd_alloc_error());
	if (env_add_extern_value(envlst, "OLDPWD", currpath) == FUNCT_ERROR)
		return (cd_alloc_error());

	if (cd_flag == BUILTIN_CD_UP)
	{
		ft_strdel(&newpath);
		newpath = getcwd(NULL, 0);
		if (newpath == NULL)
			return (cd_alloc_error());
	}
	ft_printf(">>>>>>>>>>>>>>>>>>>>\nPWD: >%s<\n", newpath);
	if (env_add_extern_value(envlst, "PWD", newpath) == FUNCT_ERROR)
		return (cd_alloc_error());
	return (FUNCT_SUCCESS);
}

static int		cd_change_dir(char *argpath, t_envlst *envlst, char cd_flag,
int print)
{
	char		*pwd;
	char		*currpath;

	pwd = env_getvalue("PWD", envlst);
	if (cd_flag == BUILTIN_CD_UL && pwd != NULL)
		currpath = ft_strdup(pwd);
	else
		currpath = getcwd(NULL, 0);

	
	struct stat	ptr;
	struct stat	ptr2;
	int			ret;
	char		*correct_path;
	char		*newpwdstr;

	newpwdstr = NULL;
	correct_path = argpath;

	#ifdef DEBUG
	ret = lstat(cd_make_new_sympath(currpath, argpath), &ptr);
	ft_printf("path: %s - ret: %d - errno: %s\n", argpath, ret, strerror(errno));
	#endif
	lstat(currpath, &ptr2);
	if (cd_flag == BUILTIN_CD_UL && (S_ISLNK(ptr.st_mode) || S_ISLNK(ptr2.st_mode)))
	{
		ft_putendl("CORRECT SYMLINK!!!!!!");
		correct_path = cd_make_new_sympath(currpath, argpath);
		newpwdstr = correct_path;
	}
	if (currpath == NULL)
		return (cd_change_dir_error(NULL));
	if (chdir(correct_path) != 0)
		return (cd_change_dir_error(argpath));
	if (print == true)
		ft_putendl(argpath);
	if (newpwdstr == NULL)
		newpwdstr = getcwd(NULL, 0);
	if (cd_post_process_var(currpath, newpwdstr, envlst, cd_flag) == FUNCT_ERROR)
		return (FUNCT_ERROR);
	ft_strdel(&currpath);
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
				*cd_flag = BUILTIN_CD_UP;
			else if (args[j][i] == 'L')
				*cd_flag = BUILTIN_CD_UL;
			else
				return (cd_invalid_option(args[j][i]));
			i++;
		}
		j++;
	}
	*countflags = j - 1;
	return (FUNCT_SUCCESS);
}

static int		cd_parse_dash(char *newpath, t_envlst *envlst, char cd_flag,
char *var)
{
	if (newpath == NULL || *newpath == '\0')
	{
		ft_eprintf("vsh: cd: %s: not set\n", var);
		return (FUNCT_ERROR);
	}
	if (ft_strequ(var, "HOME") == 1)
		return (cd_change_dir(newpath, envlst, cd_flag, false));
	else
		return (cd_change_dir(newpath, envlst, cd_flag, true));
}

int				builtin_cd(char **args, t_envlst *envlst)
{
	char	cd_flag;
	char	*newpath;
	int		flags;

	cd_flag = BUILTIN_CD_UL;
	flags = 0;
	if (cd_parse_flags(args, &cd_flag, &flags) == 0)
		return (FUNCT_ERROR);
	if (args[1 + flags] == NULL || ft_strequ(args[1 + flags], "--") == 1)
	{
		newpath = env_getvalue("HOME", envlst);
		return (cd_parse_dash(newpath, envlst, cd_flag, "HOME"));
	}
	if (ft_strequ(args[1 + flags], "-") == 1)
	{
		newpath = env_getvalue("OLDPWD", envlst);
		return (cd_parse_dash(newpath, envlst, cd_flag, "OLDPWD"));
	}
	return (cd_change_dir(args[1 + flags], envlst, cd_flag, false));
}
