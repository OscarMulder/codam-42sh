/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   glob_debug.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: mavan-he <mavan-he@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/10/13 17:39:27 by mavan-he       #+#    #+#                */
/*   Updated: 2019/10/13 20:15:57 by mavan-he      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"

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

void			glob_print_tokenlist(t_globtoken *lst)
{
	t_globtoken	*probe;
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
