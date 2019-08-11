/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   auto_get_varlst.c                                  :+:    :+:            */
/*                                                     +:+                    */
/*   By: mavan-he <mavan-he@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/08/11 15:38:10 by mavan-he       #+#    #+#                */
/*   Updated: 2019/08/11 16:37:48 by mavan-he      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"

int		auto_get_varlst(char *match, int match_len, t_envlst *envlst,
		t_list **matchlst)
{
	if (envlst == NULL)
		return (FUNCT_SUCCESS);
	if (ft_strnequ(match, envlst->var, match_len) == true &&
		auto_add_tolst(matchlst, envlst->var) == FUNCT_ERROR)
		return (FUNCT_ERROR);
	return (auto_get_varlst(match, match_len, envlst->next, matchlst));
}
