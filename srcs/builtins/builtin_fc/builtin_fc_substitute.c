/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   builtin_fc_substitute.c                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: omulder <omulder@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/09/21 15:22:42 by omulder        #+#    #+#                */
/*   Updated: 2019/09/21 16:06:52 by omulder       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"

static int	find_index(t_datahistory *history, t_fcdata *fc, int *index)
{
	if (fc->first == NULL)
		index = history->hist_start;
	else
	{
		if (fc_find_index(history, fc, fc->first, &index) == FUNCT_FAILURE)
			return (FUNCT_FAILURE);
	}
	return (FUNCT_SUCCESS);
}

static int	replace_cmd(t_datahistory *history, t_fcdata *fc, int index,
char **cmd)
{
	char	*find;
	char	*replace;

	find = NULL;
	replace = NULL;
	if (fc->replace != NULL)
	{
		replace = (ft_strchr(fc->replace, '=') + 1);
		find = ft_strsub(fc->replace, 0, ((replace - 1) - fc->replace));
		if (find == NULL)
			return (err_ret_exitcode(E_ALLOC_STR, FUNCT_FAILURE));
		*cmd = ft_strreplace(history->history[index]->str, find, replace);
		if (*cmd == NULL)
			return (err_ret_exitcode(E_ALLOC_STR, FUNCT_FAILURE));
	}
	else
		*cmd = ft_strdup(history->history[index]->str);
	return (FUNCT_SUCCESS);
}

int		fc_substitute(t_datahistory *history, t_fcdata *fc)
{
	int		index;
	char	*cmd;

	index = 0;
	cmd = NULL;
	if (find_index(history, fc, &index) == FUNCT_FAILURE)
		return (EXIT_FAILURE);
	if (replace_cmd(history, fc, index, &cmd) == FUNCT_FAILURE)
		return (EXIT_FAILURE);
	// EXECUTE CMD HERE
	return (EXIT_SUCCESS);
}
