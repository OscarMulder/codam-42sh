/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   glob_expand_word.c                                 :+:    :+:            */
/*                                                     +:+                    */
/*   By: jbrinksm <jbrinksm@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/10/07 14:54:03 by jbrinksm       #+#    #+#                */
/*   Updated: 2019/10/07 17:10:14 by jbrinksm      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"

static void	reset_glob_scanner(t_globscanner *scanner)
{
	scanner->tk_len = 0;
	scanner->tk_type = GLOB_ERROR;
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
	if (GLOB_CUR_CHAR == ']')
		glob_lexer_finish(scanner, GLOB_BRACED);
	else if (GLOB_CUR_CHAR != '\0')
		glob_lexer_changestate(scanner, &glob_lexer_state_braced);
	else
		return ;
}

void			glob_lexer_state_str(t_globscanner *scanner)
{
	scanner->tk_type = GLOB_STR;
	if (GLOB_CUR_CHAR == '\0'
	|| GLOB_CUR_CHAR == '!'
	|| GLOB_CUR_CHAR == '?'
	|| GLOB_CUR_CHAR == '['
	|| GLOB_CUR_CHAR == '*')
		return ;
}

void			glob_lexer_state_start(t_globscanner *scanner)
{
	if (GLOB_CUR_CHAR == '\0')
		return ;
	if (GLOB_CUR_CHAR == '*')
		glob_lexer_finish(scanner, GLOB_WILD);
	else if (GLOB_CUR_CHAR == '!')
		glob_lexer_finish(scanner, GLOB_WILD);
	else if (GLOB_CUR_CHAR == '?')
		glob_lexer_finish(scanner, GLOB_WILD);
	else if (GLOB_CUR_CHAR == '[')
		glob_lexer_changestate(scanner, &glob_lexer_state_braced);
	else
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

void			glob_print_list(t_globtokenlst *lst)
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
	while (probe != NULL)
	{
		ft_eprintf("%i.\ttype: %i\tchunk:\t%s\n", i, probe->tk_type,
			probe->word_chunk);
		i++;
		probe = probe->next;
	}
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

int			glob_parser(t_globtokenlst *lst)
{
	
}

int		glob_expand_word(char *word)
{
	t_globtokenlst		*lst;

	lst = NULL;
	if (word == NULL)
		return (FUNCT_ERROR);
	glob_print_list(lst);
	return (glob_lexer(&lst, word));
}
