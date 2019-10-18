/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   glob_helpers.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: mavan-he <mavan-he@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/10/17 11:47:38 by mavan-he       #+#    #+#                */
/*   Updated: 2019/10/18 17:40:27 by mavan-he      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"

void	glob_delmatch(t_globmatchlst **match)
{
	ft_strdel(&(*match)->word);
	free(*match);
	*match = NULL;
}

void	glob_del_matchlst(t_globmatchlst **matchlst)
{
	if (*matchlst == NULL)
		return ;
	glob_del_matchlst(&(*matchlst)->next);
	glob_delmatch(matchlst);
}

int		glob_add_dotslash_to_path(t_globtoken **tokenlst, char **path)
{
	if (*path == NULL)
	{
		*path = ft_strnew(0);
		if (*path == NULL)
			return (err_ret_exit(E_ALLOC_STR, EXIT_FAILURE));
	}
	while (*tokenlst != NULL && ((*tokenlst)->tk_type == GLOB_DOTDOTSLASH ||
		(*tokenlst)->tk_type == GLOB_DOTSLASH ||
		(*tokenlst)->tk_type == GLOB_SLASH))
	{
		*path = ft_strjoinfree_s1(*path, ((*tokenlst)->word_chunk));
		if (*path == NULL)
			return (err_ret_exit(E_ALLOC_STR, EXIT_FAILURE));
		*tokenlst = (*tokenlst)->next;
	}
	return (FUNCT_SUCCESS);
}

int		glob_is_dir(char *match, char *path)
{
	char	*filepath;
	int		ret;

	filepath = ft_strjoin(path, match);
	if (filepath == NULL)
		return (err_ret_exit(E_ALLOC_STR, EXIT_FAILURE));
	ret = ft_isdir(filepath);
	ft_strdel(&filepath);
	if (ret == true)
		return (FUNCT_SUCCESS);
	return (FUNCT_FAILURE);
}

int		glob_keep_dirs(t_globmatchlst **matchlst, char *path)
{
	t_globmatchlst *matchprobe;
	t_globmatchlst *next;

	if (*matchlst == NULL)
		return (FUNCT_ERROR);
	matchprobe = *matchlst;
	while (matchprobe->next != NULL)
	{
		if (glob_is_dir(matchprobe->next->word, path) != FUNCT_SUCCESS)
		{
			next = matchprobe->next->next;
			glob_delmatch(&matchprobe->next);
			matchprobe->next = next;
		}
		else
			matchprobe = matchprobe->next;
	}
	if (glob_is_dir((*matchlst)->word, path) != FUNCT_SUCCESS)
	{
		next = (*matchlst)->next;
		glob_delmatch(matchlst);
		*matchlst = next;
	}
	return (FUNCT_SUCCESS);
}
