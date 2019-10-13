/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   glob_lst_funcs.c                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: mavan-he <mavan-he@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/10/13 19:53:46 by mavan-he       #+#    #+#                */
/*   Updated: 2019/10/13 21:17:58 by mavan-he      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"

t_globtoken	*glob_tokenlstnew(char *word_chunk, int type)
{
	t_globtoken	*globtoken;

	if (word_chunk == NULL)
		return (NULL);
	globtoken = ft_memalloc(sizeof(t_globtoken));
	if (globtoken == NULL)
		return (NULL);
	globtoken->word_chunk = word_chunk;
	if (globtoken->word_chunk == NULL)
	{
		ft_memdel((void*)&globtoken);
		return (NULL);
	}
	globtoken->tk_type = type;
	globtoken->word_len = ft_strlen(globtoken->word_chunk);
	return (globtoken);
}

void			glob_tokenlstadd(t_globtoken **lst, t_globtoken *new)
{
	t_globtoken *probe;

	if (*lst == NULL)
	{
		*lst = new;
		return ;
	}
	probe = *lst;
	while (probe->next != NULL)
		probe = probe->next;
	probe->next = new;
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
	t_globmatchlst	*new;


	if (lst == NULL || word == NULL)
		return (FUNCT_ERROR);
	new = glob_matchlstnew(word);
	if (new == NULL)
		return (err_ret_exit(E_ALLOC_STR, EXIT_FAILURE));
	if (*lst == NULL)
		*lst = new;
	else
	{
		new->next = *lst;
		*lst = new;
	}
	return (FUNCT_SUCCESS);
}
