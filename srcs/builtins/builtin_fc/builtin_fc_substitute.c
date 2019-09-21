/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   builtin_fc_substitute.c                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: omulder <omulder@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/09/21 15:22:42 by omulder        #+#    #+#                */
/*   Updated: 2019/09/21 17:57:11 by omulder       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"

static int	find_index(t_datahistory *history, t_fcdata *fc, int *index)
{
	if (fc->first == NULL)
		*index = history->hist_start;
	else
	{
		if (fc_find_index(history, fc, fc->first, index) == FUNCT_FAILURE)
			return (FUNCT_FAILURE);
	}
	return (FUNCT_SUCCESS);
}

static int	replace_cmd(t_datahistory *history, t_fcdata *fc, int index,
char **cmd)
{
	char	*find;
	char	*replace;
	char	*tmp;

	find = NULL;
	replace = NULL;
	*cmd = ft_strjoin(history->history[index]->str, "\n");
	if (*cmd == NULL)
		return (FUNCT_FAILURE);
	if (fc->replace != NULL)
	{
		replace = (ft_strchr(fc->replace, '=') + 1);
		if (replace == NULL)
			return (FUNCT_FAILURE);
		find = ft_strsub(fc->replace, 0, ((replace - 1) - fc->replace));
		if (find == NULL)
			return (err_ret_exitcode(E_ALLOC_STR, FUNCT_FAILURE));
		tmp = ft_strreplace(*cmd, find, replace);
		ft_strdel(&find);
		if (tmp == NULL)
			return (err_ret_exitcode(E_ALLOC_STR, FUNCT_FAILURE));
		ft_strdel(cmd);
		*cmd = tmp;
	}
	return (FUNCT_SUCCESS);
}

int			fc_substitute(t_vshdata *data, t_datahistory *history, t_fcdata *fc)
{
	int		index;
	char	*cmd;

	index = 0;
	cmd = NULL;
	if (find_index(history, fc, &index) == FUNCT_FAILURE ||
	replace_cmd(history, fc, index, &cmd) == FUNCT_FAILURE)
	{
		ft_strdel(&cmd);
		return (EXIT_FAILURE);
	}
	data->line->line = cmd;
	history_replace_last(history->history, &cmd);
	shell_one_line(data);
	return (EXIT_SUCCESS);
}
