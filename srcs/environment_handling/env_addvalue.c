/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   env_addvalue.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: omulder <omulder@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/07/30 15:56:30 by omulder        #+#    #+#                */
/*   Updated: 2019/07/30 16:22:18 by omulder       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"

int		env_add_extern_value(t_envlst *envlst, char *name, char *value)
{
	t_envlst	*newitem;
	char		*var;

	var = ft_strjoinfree_s2(name, ft_strjoin("=", value));
	if (var == NULL)
		return (FUNCT_ERROR);
	newitem = env_lstnew(var, ENV_EXTERN);
	ft_strdel(&var);
	if (newitem == NULL)
		return (FUNCT_ERROR);
	env_lstadd_to_sortlst(envlst, newitem);
	return (FUNCT_SUCCESS);
}
