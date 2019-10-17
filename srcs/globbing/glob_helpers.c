/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   glob_helpers.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: mavan-he <mavan-he@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/10/17 11:47:38 by mavan-he       #+#    #+#                */
/*   Updated: 2019/10/17 19:11:51 by mavan-he      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"

void		glob_delmatch(t_globmatchlst **match)
{
	ft_strdel(&(*match)->word);
	free(*match);
	*match = NULL;
}

int		glob_add_dotslash_to_path(t_globtoken **tokenlst, char **path)
{
	if (*path == NULL)
	{
		*path = ft_strnew(0);
		if (*path == NULL)
			return (FUNCT_ERROR); // error
	}
	while (*tokenlst != NULL && ((*tokenlst)->tk_type == GLOB_DOTDOTSLASH ||
		(*tokenlst)->tk_type == GLOB_DOTSLASH ||
		(*tokenlst)->tk_type == GLOB_SLASH))
	{
		*path = ft_strjoinfree_s1(*path, ((*tokenlst)->word_chunk));
		if (*path == NULL)
			return (FUNCT_ERROR); // error
		*tokenlst = (*tokenlst)->next;
	}
	return (FUNCT_SUCCESS);
}

/* void	glob_keep_dirs(t_globmatchlst **matchlst, char *path)
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
	return (FUNCT_SUCCESS);
} */