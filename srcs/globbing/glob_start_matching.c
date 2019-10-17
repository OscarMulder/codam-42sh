/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   glob_start_matching.c                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: mavan-he <mavan-he@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/10/17 18:19:03 by mavan-he       #+#    #+#                */
/*   Updated: 2019/10/17 19:12:01 by mavan-he      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"

#include <dirent.h> // double check
#include <limits.h> // double check

int			glob_getmatchlist(t_globmatchlst **matchlst, char *path)
{
	DIR				*dir;
	struct dirent	*rdir;

	dir = opendir(path);
	if (dir == NULL)
	{
		ft_eprintf(E_OPEN_DIR, path);
		return (err_ret_exit(NULL, EXIT_FAILURE));
	}
	rdir = readdir(dir);
	while (rdir != NULL)
	{
		if (!(ft_strequ(rdir->d_name, ".") || ft_strequ(rdir->d_name, "..")))
		{
			if (glob_matchlstadd(matchlst, rdir->d_name) == FUNCT_ERROR)
				return (FUNCT_ERROR);
		}
		rdir = readdir(dir);
	}
	return (FUNCT_SUCCESS);
}

int			glob_matcher(t_globtoken *tokenlst, t_globmatchlst **matchlst)
{
	t_globmatchlst *matchprobe;
	t_globmatchlst *next;

	if (*matchlst == NULL)
		return (FUNCT_SUCCESS);
	matchprobe = *matchlst;
	while (matchprobe->next != NULL)
	{
		if (glob_start_matching(tokenlst, *(matchprobe->next)) != FUNCT_SUCCESS)
		{
			next = matchprobe->next->next;
			glob_delmatch(&matchprobe->next);
			matchprobe->next = next;
		}
		else
			matchprobe = matchprobe->next;
	}
	if (glob_start_matching(tokenlst, **matchlst) != FUNCT_SUCCESS)
	{
		next = (*matchlst)->next;
		glob_delmatch(matchlst);
		*matchlst = next;
	}
	return (FUNCT_SUCCESS);
}

int			glob_loop_matcher(t_globtoken *tokenlst, char *path)
{
	t_globmatchlst	*matchlst;
	char			*new_path;
	char			*add_path;

	matchlst = NULL;
	add_path = NULL;
	if (glob_getmatchlist(&matchlst, path) == FUNCT_ERROR)
		return (FUNCT_ERROR);
	if (glob_matcher(tokenlst, &matchlst) == FUNCT_ERROR)
		return (FUNCT_ERROR);
	if (matchlst == NULL)
		return (FUNCT_FAILURE);
	while (tokenlst != NULL && tokenlst->tk_type != GLOB_SLASH)
		tokenlst = tokenlst->next;
	if (glob_add_dotslash_to_path(&tokenlst, &add_path) == FUNCT_ERROR)
		return (FUNCT_ERROR);
	// if (tokenlst != NULL)
	// 	glob_keep_dirs(&matchlst);
	while (matchlst != NULL)
	{
		new_path = ft_strjoin(matchlst->word, add_path);
		new_path = ft_strjoinfree_s2(path, new_path);
		if (tokenlst != NULL)
		{
			if (glob_loop_matcher(tokenlst, new_path) == FUNCT_ERROR)
				return (FUNCT_ERROR);
		}
		else
			ft_printf("%s   ", new_path);
		matchlst = matchlst->next;
	}
	return (FUNCT_SUCCESS);
}