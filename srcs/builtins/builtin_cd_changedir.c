/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   builtin_cd_changedir.c                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: jbrinksm <jbrinksm@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/08/02 15:03:51 by jbrinksm       #+#    #+#                */
/*   Updated: 2019/08/02 15:38:08 by tde-jong      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"

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
	if (env_add_extern_value(envlst, "PWD", newpath) == FUNCT_ERROR)
		return (cd_alloc_error());
	return (FUNCT_SUCCESS);
}

static char	*cd_return_symlink_path(char *currpath, char *argpath,
					char cd_flag)
{
	char		*newpath;

	newpath = builtin_cd_create_newpath_wrap(currpath, argpath);
	if (cd_flag & BUILTIN_CD_UL)
		return (newpath);
	return (NULL);
}

static int	return_and_free(int ret, char **newpath, char **currpath)
{
	ft_strdel(currpath);
	ft_strdel(newpath);
	return (ret);
}

int			builtin_cd_change_dir(char *argpath, t_envlst *envlst, char cd_flag,
				int print)
{
	char		*pwd;
	char		*currpath;
	char		*newpath;

	newpath = NULL;
	pwd = env_getvalue("PWD", envlst);
	if (cd_flag & BUILTIN_CD_UL && pwd != NULL)
		currpath = ft_strdup(pwd);
	else
		currpath = getcwd(NULL, 0);
	if (currpath == NULL)
		return (cd_change_dir_error(NULL, argpath, NULL, NULL));
	if (cd_flag & BUILTIN_CD_UL)
		newpath = cd_return_symlink_path(currpath, argpath, cd_flag);
	if (newpath == NULL && chdir(argpath) == -1)
		return (cd_change_dir_error(argpath, argpath, &newpath, &currpath));
	else if (newpath != NULL && chdir(newpath) == -1)
		return (cd_change_dir_error(newpath, argpath, &newpath, &currpath));
	if (print == true)
		ft_putendl(argpath);
	if (newpath == NULL)
		newpath = getcwd(NULL, 0);
	if (cd_post_process_var(currpath, newpath, envlst, cd_flag) == FUNCT_ERROR)
		return (return_and_free(FUNCT_ERROR, &newpath, &currpath));
	return (return_and_free(FUNCT_SUCCESS, &newpath, &currpath));
}
