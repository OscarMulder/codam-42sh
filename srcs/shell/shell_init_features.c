/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   shell_init_features.c                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: rkuijper <rkuijper@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/09/04 10:25:31 by rkuijper       #+#    #+#                */
/*   Updated: 2019/09/04 10:38:05 by mavan-he      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"

t_vshdatahashtable	*shell_init_vshdatahashtable(void)
{
	t_vshdatahashtable	*hashtable;

	hashtable = ft_memalloc(sizeof(t_vshdatahashtable));
	if (hashtable == NULL)
		return (NULL);
	return (hashtable);
}

t_vshdatahistory	*shell_init_vshdatahistory(void)
{
	t_vshdatahistory	*history;

	history = ft_memalloc(sizeof(t_vshdatahistory));
	if (history == NULL)
		return (NULL);
	return (history);
}

t_vshdataalias		*shell_init_vshdataalias(void)
{
	t_vshdataalias	*alias;

	alias = ft_memalloc(sizeof(t_vshdataalias));
	if (alias == NULL)
		return (NULL);
	alias->alias_file = ALIASFILENAME;
	return (alias);
}
