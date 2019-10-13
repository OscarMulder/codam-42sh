/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   glob_expand_word.c                                 :+:    :+:            */
/*                                                     +:+                    */
/*   By: jbrinksm <jbrinksm@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/10/07 14:54:03 by jbrinksm       #+#    #+#                */
/*   Updated: 2019/10/13 21:27:02 by mavan-he      ########   odam.nl         */
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

int			glob_matching_wild(t_globtoken *tokenprobe, t_globmatchlst match)
{
	while (match.index <= match.word_len)
	{
		if (glob_start_matching(tokenprobe->next, match) == FUNCT_SUCCESS)
			return (FUNCT_SUCCESS);
		match.index++;
	}
	return (FUNCT_FAILURE);
}

int			glob_matching_braceneg(t_globtoken *tokenprobe, t_globmatchlst match)
{
	int i;

	i = 1;
	while (tokenprobe->word_chunk[i + 1] != '\0')
	{
		if (tokenprobe->word_chunk[i] == match.word[match.index])
			return (FUNCT_FAILURE);
		i++;
	}
	match.index++;
	return (glob_start_matching(tokenprobe->next, match));
}

int			glob_matching_bracepos(t_globtoken *tokenprobe, t_globmatchlst match)
{
	int i;

	i = 1;
	while (tokenprobe->word_chunk[i + 1] != '\0')
	{
		if (tokenprobe->word_chunk[i] == match.word[match.index])
		{
			match.index++;
			return (glob_start_matching(tokenprobe->next, match));
		}
		i++;
	}
	return (FUNCT_FAILURE);
}

int			glob_start_matching(t_globtoken *tokenprobe, t_globmatchlst match)
{
	if (match.index == match.word_len && tokenprobe == NULL)
		return (FUNCT_SUCCESS);
	else if (tokenprobe == NULL)
		return (FUNCT_FAILURE);

	if (tokenprobe->tk_type == GLOB_STR || tokenprobe->tk_type == GLOB_SLASH
		|| tokenprobe->tk_type == GLOB_DOTSLASH || tokenprobe->tk_type == GLOB_DOTDOTSLASH)
	{
		if (ft_strnequ(tokenprobe->word_chunk, &match.word[match.index],
			tokenprobe->word_len) == false)
			return (FUNCT_FAILURE);
		else
		{
			match.index += tokenprobe->word_len;
			return (glob_start_matching(tokenprobe->next, match));
		}
	}

	else if (tokenprobe->tk_type == GLOB_QUEST)
	{
		if (match.word[match.index] == '\0')
			return (FUNCT_FAILURE);
		else
		{
			match.index += tokenprobe->word_len;
			return (glob_start_matching(tokenprobe->next, match));
		}
	}

	else if (tokenprobe->tk_type == GLOB_BRACEPOS)
		return (glob_matching_bracepos(tokenprobe, match));

	else if (tokenprobe->tk_type == GLOB_BRACENEG)
		return (glob_matching_braceneg(tokenprobe, match));

	else if (tokenprobe->tk_type == GLOB_WILD)
		return (glob_matching_wild(tokenprobe, match));

	return (FUNCT_SUCCESS);
}

int			glob_matcher(t_globtoken *tokenlst, t_globmatchlst *matchlst)
{
	t_globmatchlst *matchprobe;

	matchprobe = matchlst;
	while (matchprobe != NULL)
	{
		if (glob_start_matching(tokenlst, *matchprobe) == FUNCT_SUCCESS)
			matchprobe->matched = true;
		matchprobe = matchprobe->next;
	}
	return (FUNCT_SUCCESS);
}

int			glob_getpotentials_abs(t_globmatchlst **matchlst, t_globtoken *tokenlst, char *path)
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
}

int			glob_getpotentials_cwd(t_globmatchlst **matchlst)
{
	DIR				*dir;
	struct dirent	*rdir;
	char			*cwd;

	cwd = getcwd(NULL, 0);
	if (cwd == NULL)
		return (FUNCT_ERROR);
	dir = opendir(cwd);
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

int			glob_getmatchlist(t_globmatchlst **matchlst, t_globtoken *tokenlst)
{
	t_globtoken *probe;

	probe = tokenlst;
	if (probe->tk_type == GLOB_SLASH)
		glob_getpotentials_abs(matchlst, tokenlst, NULL);
	else
		glob_getpotentials_cwd(matchlst);
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
	if (glob_getmatchlist(&matchlst, tokenlst) == FUNCT_ERROR)
		return (FUNCT_ERROR);
	glob_print_matchlist(matchlst); // debug
	if (glob_matcher(tokenlst, matchlst) == FUNCT_ERROR)
		return (FUNCT_ERROR);
	glob_print_matches(matchlst); // debug
	return (FUNCT_SUCCESS);
}
