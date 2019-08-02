/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   exec_tilde_expansion.c                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: mavan-he <mavan-he@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/08/02 13:50:51 by mavan-he       #+#    #+#                */
/*   Updated: 2019/08/02 18:21:28 by mavan-he      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

/*
**	~ or ~/ is being expanded to HOME or HOME/ respectively
**	a ~ followed by anything other than / is not expanded
*/

#include "vsh.h"

static int	add_home_to_value(t_ast *node, int *i, char *home)
{
	char	*new_value;
	int		len_home;

	len_home = ft_strlen(home);
	new_value = ft_strnew(sizeof(char) * (len_home + ft_strlen(node->value)));
	if (new_value == NULL)
		return (FUNCT_ERROR);
	if (node->type == ASSIGN)
		ft_strncpy(new_value, node->value,
		ft_strchr(node->value, '=') - node->value);
	ft_strcat(new_value, home);
	ft_strcat(new_value, &node->value[*i + 1]);
	*i += len_home;
	ft_strdel(&node->value);
	node->value = new_value;
	return (FUNCT_SUCCESS);
}

int			exec_tilde_expansion(t_ast *node, int *i)
{
	char	*home;

	if (node->type == ASSIGN)
		*i = (ft_strchr(node->value, '=') - node->value) + 1;
	if (node->value[*i] != '~' || (node->value[*i + 1] != '/' &&
		node->value[*i + 1] != '\0'))
		return (FUNCT_FAILURE);
	home = getenv("HOME");
	if (home == NULL)
	{
		ft_eprintf("vsh: tilde: failed to get home directory\n");
		return (FUNCT_ERROR);
	}
	return (add_home_to_value(node, i, home));
}
