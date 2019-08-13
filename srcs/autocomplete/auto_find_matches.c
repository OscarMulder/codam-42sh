/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   auto_find_matches.c                                :+:    :+:            */
/*                                                     +:+                    */
/*   By: mavan-he <mavan-he@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/08/12 20:20:16 by mavan-he       #+#    #+#                */
/*   Updated: 2019/08/13 17:01:34 by mavan-he      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"

static int	cut_path_from_match(char **match, int i)
{
	char *new_match;

	new_match = ft_strdup(&(*match)[i]);
	if (new_match == NULL)
	{
		ft_eprintf("vsh: failed to allocate enough memory\n");
		return (FUNCT_ERROR);
	}
	ft_strdel(match);
	*match = new_match;
	return (FUNCT_SUCCESS);
}

int			auto_find_filelst(char **match, t_list **matchlst)
{
	int		match_len;
	char	*path;
	int		i;
	int		ret;

	match_len = ft_strlen(*match);
	i = match_len - 1;
	while (i >= 0)
	{
		if (tools_isidentifierchar((*match)[i]) == false && (*match)[i] != '.')
			break;
		i--;
	}
 	if (i < 0)
		path = getcwd(NULL, 0);
	else
		path = ft_strndup((*match), match_len - (match_len - (i + 1)));
	if (path == NULL || cut_path_from_match(match, i + 1) == FUNCT_ERROR)
		return (FUNCT_ERROR);
	ft_printf("<<<<< Path = %s : ReMatch = %s >>>>>> \n", path, *match); // debugging
	ret = auto_get_filelst(*match, path, matchlst);
	ft_strdel(&path);
	return (ret);
}


int			auto_find_matches(t_vshdata *vshdata, char **match,
			t_list **matchlst, int state)
{
	if (state == STATE_CMD && auto_get_cmdlst(*match, vshdata->envlst,
		matchlst) == FUNCT_ERROR)
		return (FUNCT_ERROR);
	if (state == STATE_VAR && auto_get_varlst(*match, ft_strlen(*match),
		vshdata->envlst, matchlst) == FUNCT_ERROR)
		return (FUNCT_ERROR);
	if (state == STATE_FILE && auto_find_filelst(match, matchlst) == FUNCT_ERROR)
		return (FUNCT_ERROR);
	return (FUNCT_SUCCESS);
}
