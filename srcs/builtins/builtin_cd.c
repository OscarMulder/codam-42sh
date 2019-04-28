/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   builtin_cd.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: mavan-he <mavan-he@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/04/25 17:17:25 by mavan-he       #+#    #+#                */
/*   Updated: 2019/04/28 13:22:26 by mavan-he      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

/*
** WORK IN PROGRESS
**
**
** CD changes directory
** 1. no args:
** change to homedir
** 2. 1 arg
** change dir to arg[1]
*/

#include "vsh.h"

int			builtin_cd(char **args, char **env)
{
	(void)args;
	(void)env;
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