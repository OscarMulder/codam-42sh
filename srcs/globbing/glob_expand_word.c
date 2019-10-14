/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   glob_expand_word.c                                 :+:    :+:            */
/*                                                     +:+                    */
/*   By: jbrinksm <jbrinksm@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/10/07 14:54:03 by jbrinksm       #+#    #+#                */
/*   Updated: 2019/10/14 17:17:40 by mavan-he      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */


#include <dirent.h> // double check
#include <limits.h> // double check
#include "vsh.h"

/* t_globmatchlst	*glob_strarr_to_lst(char **items)
{
	t_globmatchlst	*lst;
	t_globmatchlst	*probe;
	int				i;

	if (items == NULL || *items == NULL)
		return (NULL);
	lst = glob_matchlstnew(items[0]);
	if (lst == NULL)
		return (NULL);
	i = 1;
	probe = lst;
	while (items[i] != NULL)
	{
		probe->next = glob_matchlstnew(items[i]);
		if (probe->next == NULL)
		{
			//free list;
			return (NULL);
		}
		probe = probe->next;
		i++;
	}
	return (lst);
} */

/* int			glob_getpotentials_abs(t_globmatchlst **matchlst, t_globtoken *tokenlst, char *path)
{
	t_globtoken	*probe;
//	char			*path;

	(void)matchlst;
	path = ft_strnew(PATH_MAX);
	if (path == NULL)
		return (FUNCT_ERROR);
	path[0] = '/';
	probe = tokenlst;
	while (probe->tk_type == GLOB_SLASH || probe->tk_type == GLOB_DOTSLASH
		|| probe->tk_type == GLOB_DOTDOTSLASH)
		probe = probe->next;
	if (probe->tk_type == GLOB_STR)
		ft_strcat(path, probe->word_chunk);
	return (FUNCT_SUCCESS);
} */

void		glob_delmatch(t_globmatchlst **match)
{
	ft_strdel(&(*match)->word);
	free(*match);
	*match = NULL;
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

int			glob_init_matchlst(t_globmatchlst **matchlst, t_globtoken *tokenlst)
{
	char		*path;

	if (tokenlst->tk_type == GLOB_SLASH)
		path = ft_strdup("/");
	else if (tokenlst->tk_type == GLOB_DOTDOTSLASH)
		path = ft_strdup("../");
	else if (tokenlst->tk_type == GLOB_DOTSLASH)
		path = ft_strdup("./");
	else
		path = getcwd(NULL, 0);
	if (path == NULL)
		return (FUNCT_ERROR);
	if (glob_getmatchlist(matchlst, path) == FUNCT_ERROR)
		return (FUNCT_ERROR);
	return (FUNCT_SUCCESS);
}

int		glob_expand_word(char *word)
{
	t_globtoken		*tokenlst;
	t_globmatchlst		*matchlst;

	if (word == NULL)
		return (FUNCT_ERROR);
	tokenlst = NULL;
	matchlst = NULL;
	if (glob_lexer(&tokenlst, word) == FUNCT_ERROR)
		return (FUNCT_ERROR);
	glob_print_tokenlist(tokenlst); // debug
	if (glob_init_matchlst(&matchlst, tokenlst) == FUNCT_ERROR)
		return (FUNCT_ERROR);
	glob_print_matchlist(matchlst); // debug
	if (glob_matcher(tokenlst, &matchlst) == FUNCT_ERROR)
		return (FUNCT_ERROR);
	glob_print_matches(matchlst); // debug
	return (FUNCT_SUCCESS);
}
