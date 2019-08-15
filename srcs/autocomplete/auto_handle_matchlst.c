/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   auto_handle_matchlst.c                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: mavan-he <mavan-he@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/08/13 11:00:12 by mavan-he       #+#    #+#                */
/*   Updated: 2019/08/15 15:41:11 by omulder       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"

int		auto_handle_matchlst(t_vshdata *vshdata, t_inputdata *data,
		char *match, t_list **matchlst)
{
	int		lst_len;

	if (*matchlst == NULL)
		return (FUNCT_FAILURE);
	lst_len = ft_lstlen(*matchlst);
	if (lst_len == 1)
		return (auto_add_match_toline(
				match, (char *)(*matchlst)->content, &vshdata->line, data));
	else if (lst_len > 100)
		return (auto_big_lst(*matchlst, lst_len));
	else
		return (auto_small_lst(match, *matchlst, vshdata, data));
	
}
