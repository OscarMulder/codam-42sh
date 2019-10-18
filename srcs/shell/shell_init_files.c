/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   shell_init_files.c                                 :+:    :+:            */
/*                                                     +:+                    */
/*   By: omulder <omulder@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/07/27 12:39:59 by omulder        #+#    #+#                */
/*   Updated: 2019/10/15 12:16:31 by omulder       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"

int		shell_init_files(t_vshdata *data)
{
	char	*homedir;

	homedir = env_getvalue("HOME", data->envlst);
	if (homedir == NULL)
		return (err_ret(E_HOME_NOTSET_STR));
	data->alias->alias_file =
	ft_strjoinfree_s2(homedir, ft_strjoin("/", ALIASFILENAME));
	if (data->alias->alias_file == NULL)
		return (err_ret(E_ALLOC_STR));
	return (FUNCT_SUCCESS);
}
