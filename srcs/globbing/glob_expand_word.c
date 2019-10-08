/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   glob_expand_word.c                                 :+:    :+:            */
/*                                                     +:+                    */
/*   By: jbrinksm <jbrinksm@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/10/07 14:54:03 by jbrinksm       #+#    #+#                */
/*   Updated: 2019/10/08 18:06:53 by jbrinksm      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"

static void	reset_glob_scanner(t_globscanner *scanner)
{
	scanner->tk_len = 0;
	scanner->tk_type = GLOB_ERROR;
	scanner->flags = 0;
}

t_globtokenlst	*glob_newlst(char *word_chunk, int type)
{
	t_globtokenlst	*globtoken;

	if (word_chunk == NULL)
		return (NULL);
	globtoken = ft_memalloc(sizeof(t_globtokenlst));
	if (globtoken == NULL)
		return (NULL);
	globtoken->word_chunk = ft_strdup(word_chunk);
	if (globtoken->word_chunk == NULL)
	{
		ft_memdel((void*)&globtoken);
		return (NULL);
	}
	globtoken->tk_type = type;
	globtoken->word_len = ft_strlen(globtoken->word_chunk);
	return (globtoken);
}

int				glob_addtolst(t_globtokenlst **lst, t_globtokenlst *new)
{
	t_globtokenlst *probe;

	if (new == NULL || lst == NULL)
		return (FUNCT_ERROR);
	if (*lst == NULL)
	{
		*lst = new;
		return (FUNCT_SUCCESS);
	}
	probe = *lst;
	while (probe->next != NULL)
		probe = probe->next;
	probe->next = new;
	return (FUNCT_SUCCESS);
}

void			glob_lexer_addchar(t_globscanner *scanner)
{
	scanner->tk_len++;
	scanner->word_index++;
}

void			glob_lexer_finish(t_globscanner *scanner, t_globtokens type)
{
	glob_lexer_addchar(scanner);
	scanner->tk_type = type;
}

void			glob_lexer_changestate(t_globscanner *scanner,
					void (*state)(t_globscanner *scanner))
{
	glob_lexer_addchar(scanner);
	state(scanner);
}

void			glob_lexer_state_braced(t_globscanner *scanner)
{
	if (GLOB_CUR_CHAR == ']' && scanner->flags & GLOB_F_NEG)
		glob_lexer_finish(scanner, GLOB_BRACENEG);
	else if (GLOB_CUR_CHAR == ']')
		glob_lexer_finish(scanner, GLOB_BRACEPOS);
	else if (GLOB_CUR_CHAR == '!' && scanner->tk_len == 1)
	{
		scanner->flags |= GLOB_F_NEG;
		glob_lexer_changestate(scanner, &glob_lexer_state_braced);
	}
	else if (GLOB_CUR_CHAR != '\0')
		glob_lexer_changestate(scanner, &glob_lexer_state_braced);
	else
		return ;
}

void			glob_lexer_state_esc(t_globscanner *scanner)
{
	if (GLOB_CUR_CHAR == '\0')
		return ;
	else
		glob_lexer_changestate(scanner, &glob_lexer_state_str);
}

void			glob_lexer_state_str(t_globscanner *scanner)
{
	scanner->tk_type = GLOB_STR;
	if (GLOB_CUR_CHAR == '\\')
		glob_lexer_changestate(scanner, &glob_lexer_state_esc);
	else if (GLOB_CUR_CHAR == '\0'
	|| GLOB_CUR_CHAR == '?'
	|| GLOB_CUR_CHAR == '['
	|| GLOB_CUR_CHAR == '*'
	|| GLOB_CUR_CHAR == '/')
		return ;
	else
		glob_lexer_changestate(scanner, &glob_lexer_state_str);
}

void			glob_lexer_state_start(t_globscanner *scanner)
{
	if (GLOB_CUR_CHAR == '*')
		glob_lexer_finish(scanner, GLOB_WILD);
	else if (GLOB_CUR_CHAR == '?')
		glob_lexer_finish(scanner, GLOB_QUEST);
	else if (GLOB_CUR_CHAR == '/')
		glob_lexer_finish(scanner, GLOB_SLASH);
	else if (ft_strnequ(&GLOB_CUR_CHAR, "./", 2) == true)
	{
		scanner->tk_len++;
		scanner->word_index++;
		glob_lexer_finish(scanner, GLOB_DOTSLASH);
	}
	else if (ft_strnequ(&GLOB_CUR_CHAR, "../", 3) == true)
	{
		scanner->tk_len += 2;
		scanner->word_index += 2;
		glob_lexer_finish(scanner, GLOB_DOTDOTSLASH);
	}
	else if (GLOB_CUR_CHAR == '[')
		glob_lexer_changestate(scanner, &glob_lexer_state_braced);
	else if (GLOB_CUR_CHAR != '\0')
		glob_lexer_changestate(scanner, &glob_lexer_state_str);
}

int				glob_add_scanned_token(t_globtokenlst **lst,
				t_globscanner *scanner)
{
	t_globtokens	type;
	int				tk_start;
	char			*word_chunk;

	tk_start = scanner->word_index - scanner->tk_len;
	type = scanner->tk_type;
	if (tk_start < 0 || type == GLOB_ERROR || scanner->tk_len < 1)
		return (FUNCT_ERROR);
	word_chunk = ft_strndup(&scanner->word[tk_start], scanner->tk_len);
	if (word_chunk == NULL)
		return (FUNCT_ERROR);
	glob_addtolst(lst, glob_newlst(word_chunk, scanner->tk_type));
	return (FUNCT_SUCCESS);
}

void			glob_print_tokenlist(t_globtokenlst *lst)
{
	t_globtokenlst	*probe;
	int				i;

	probe = lst;
	i = 1;
	if (probe == NULL)
	{
		ft_eprintf("NULL lst\n");
		return ;
	}
	ft_eprintf("TOKENS:\n");
	while (probe != NULL)
	{
		ft_eprintf("%i.\ttype: %i\tchunk:\t%s\n", i, probe->tk_type,
			probe->word_chunk);
		i++;
		probe = probe->next;
	}
	ft_eprintf("\n");
}

int				glob_lexer(t_globtokenlst **lst, char *word)
{
	t_globscanner		*scanner;

	scanner = ft_memalloc(sizeof(t_globscanner));
	if (scanner == NULL)
		return (FUNCT_ERROR);
	scanner->word = word;
	while (GLOB_CUR_CHAR != '\0')
	{
		glob_lexer_state_start(scanner);
		glob_add_scanned_token(lst, scanner);
		reset_glob_scanner(scanner);
	}
	return (FUNCT_SUCCESS);
}

t_globmatchlst	*glob_matchlstnew(char *item)
{
	t_globmatchlst *new;

	new = ft_memalloc(sizeof(t_globmatchlst));
	if (new == NULL)
		return (NULL);
	new->word = item;
	if (new->word != NULL)
		new->word_len = ft_strlen(new->word);
	return (new);
}

int				glob_matchlstadd(t_globmatchlst **lst, char *word)
{
	t_globmatchlst	*probe;

	if (lst == NULL || word == NULL)
		return (FUNCT_ERROR);
	if (*lst == NULL)
	{
		*lst = glob_matchlstnew(word);
		if (*lst == NULL)
			return (FUNCT_ERROR);
	}
	else
	{
		probe = *lst;
		while (probe->next != NULL)
			probe = probe->next;
		probe->next = glob_matchlstnew(word);
		if (probe->next == NULL)
		{
			//free
			return (FUNCT_ERROR);
		}
	}
	return (FUNCT_SUCCESS);
}

t_globmatchlst	*glob_strarr_to_lst(char **items)
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
}

void			glob_print_matchlist(t_globmatchlst *lst)
{
	t_globmatchlst	*probe;
	int				i;

	probe = lst;
	i = 1;
	if (probe == NULL)
	{
		ft_eprintf("NULL lst\n");
		return ;
	}
	ft_eprintf("ITEMS:\n");
	while (probe != NULL)
	{
		ft_eprintf("%i:\t%s\n", i, probe->word);
		i++;
		probe = probe->next;
	}
	ft_eprintf("\n");
}

int			glob_matching_wild(t_globtokenlst *tokenprobe, t_globmatchlst match)
{
	while (match.index <= match.word_len)
	{
		if (glob_start_matching(tokenprobe->next, match) == FUNCT_SUCCESS)
			return (FUNCT_SUCCESS);
		match.index++;
	}
	return (FUNCT_FAILURE);
}

int			glob_matching_braceneg(t_globtokenlst *tokenprobe, t_globmatchlst match)
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

int			glob_matching_bracepos(t_globtokenlst *tokenprobe, t_globmatchlst match)
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

int			glob_start_matching(t_globtokenlst *tokenprobe, t_globmatchlst match)
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

int			glob_matcher(t_globtokenlst *tokenlst, t_globmatchlst *matchlst)
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

#include <dirent.h>
#include <limits.h>

// int			glob_getpotentials_rel(t_globmatchlst **matchlst, char *word)
// {

// }

int			glob_getpotentials_abs_browse(t_globmatchlst **matchlst, t_globtokenlst *tokenlst)
{
	
}

int			glob_getpotentials_abs(t_globmatchlst **matchlst, t_globtokenlst *tokenlst, char *path)
{
	t_globtokenlst	*probe;
	char			*path;

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

int			glob_getmatchlist(t_globmatchlst **matchlst, t_globtokenlst *tokenlst)
{
	t_globtokenlst *probe;

	probe = tokenlst;
	if (probe->tk_type == GLOB_SLASH)
		glob_getpotentials_abs(matchlst);
	else
		glob_getpotentials_cwd(matchlst);
	return (FUNCT_SUCCESS);
}

void			glob_print_matches(t_globmatchlst *lst)
{
	t_globmatchlst	*probe;
	int				i;

	probe = lst;
	i = 1;
	if (probe == NULL)
	{
		ft_eprintf("NULL lst\n");
		return ;
	}
	ft_eprintf("MATCHES:\n");
	while (probe != NULL)
	{
		if (probe->matched == true)
			ft_eprintf("%i.\tmatch:\t%s\n", i, probe->word);
		i++;
		probe = probe->next;
	}
}

int		glob_expand_word(char *word)
{
	t_globtokenlst		*tokenlst;
	t_globmatchlst		*matchlst;

	if (word == NULL)
		return (FUNCT_ERROR);
	tokenlst = NULL;
	matchlst = NULL;
	glob_lexer(&tokenlst, word);
	glob_print_tokenlist(tokenlst);
	glob_getmatchlist(&matchlst, tokenlst);
	glob_print_matchlist(matchlst);
	glob_matcher(tokenlst, matchlst);
	glob_print_matches(matchlst);
	return (FUNCT_SUCCESS);
}
