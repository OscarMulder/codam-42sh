/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   glob_start_matching.c                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: mavan-he <mavan-he@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/10/17 18:19:03 by mavan-he       #+#    #+#                */
/*   Updated: 2019/10/18 18:11:29 by mavan-he      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"
#include <dirent.h>

int			glob_getmatchlist(t_globmatchlst **matchlst, char *path)
{
	DIR				*dir;
	struct dirent	*rdir;

	dir = opendir(path);
	if (dir == NULL)
		return (FUNCT_FAILURE);
	rdir = readdir(dir);
	while (rdir != NULL)
	{
		if (!(ft_strequ(rdir->d_name, ".") || ft_strequ(rdir->d_name, "..")))
		{
			if (glob_matchlstadd(matchlst, rdir->d_name) == FUNCT_ERROR)
			{
				closedir(dir);
				return (FUNCT_ERROR);
			}
		}
		rdir = readdir(dir);
	}
	closedir(dir);
	return (FUNCT_SUCCESS);
}

void		glob_matcher(t_globtoken *tokenlst, t_globmatchlst **matchlst)
{
	t_globmatchlst *matchprobe;
	t_globmatchlst *next;

	if (*matchlst == NULL)
		return ;
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
}

int			glob_delmatchlst_ret_err(t_globmatchlst **matchlst)
{
	glob_del_matchlst(matchlst);
	return (FUNCT_ERROR);
}

int			glob_continue_check(t_globmatchlst *matchlst, t_ast **ast, t_globtoken *tokenlst, char *path,
			int cwd_len)
{
	char			*path_suffix;
	char			*new_path;

	path_suffix = NULL;
	if (glob_add_dotslash_to_path(&tokenlst, &path_suffix) == FUNCT_ERROR)
		return (FUNCT_ERROR);
	while (matchlst != NULL)
	{
		new_path = ft_strjoinfree_s2(path, ft_strjoin(matchlst->word, path_suffix));
		if (tokenlst != NULL)
		{
			if (glob_loop_matcher(ast, tokenlst, new_path, cwd_len) == FUNCT_ERROR)
				return (FUNCT_ERROR);
		}
		else
		{
			if (glob_ast_add_left(ast, &new_path[cwd_len], WORD, 0) == FUNCT_ERROR)
				return (err_ret_exit(E_ALLOC_STR, EXIT_FAILURE));
		}
		ft_strdel(&new_path);
		matchlst = matchlst->next;
	}
	return (FUNCT_SUCCESS);
}

int			glob_loop_matcher(t_ast **ast, t_globtoken *tokenlst, char *path,
			int cwd_len)
{
	t_globmatchlst	*matchlst;
	int				ret;

	matchlst = NULL;
	ret = glob_getmatchlist(&matchlst, path);
	if (ret != FUNCT_SUCCESS)
		return (ret);
	glob_matcher(tokenlst, &matchlst);
	if (matchlst == NULL)
		return (FUNCT_FAILURE);
	while (tokenlst != NULL && tokenlst->tk_type != GLOB_SLASH)
		tokenlst = tokenlst->next;
	if (tokenlst != NULL && glob_keep_dirs(&matchlst, path) == FUNCT_ERROR)
		return (glob_delmatchlst_ret_err(&matchlst));
	ret = glob_continue_check(matchlst, ast, tokenlst, path, cwd_len);
	glob_del_matchlst(&matchlst);
	return (ret);
}
