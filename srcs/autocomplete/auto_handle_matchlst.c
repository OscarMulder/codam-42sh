/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   auto_handle_matchlst.c                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: mavan-he <mavan-he@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/08/13 11:00:12 by mavan-he       #+#    #+#                */
/*   Updated: 2019/08/13 17:07:14 by mavan-he      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"

int		auto_handle_matchlst(t_vshdata *vshdata, int *i, char *match, t_list **matchlst)
{
	int		lst_len;
	t_list	*tmp;

	if (*matchlst == NULL)
		return (FUNCT_FAILURE);
	(void)match;
	lst_len = ft_lstlen(*matchlst);
	tmp = *matchlst;
	while (tmp != NULL)
	{
		ft_putendl((char *)tmp->content);
		tmp = tmp->next;
	}
	ft_printf("<<<<<<<<< list up, line down >>>>>>>>>\n"); // debug
	if (lst_len == 1)
		auto_add_match_toline(match, (char *)(*matchlst)->content, &vshdata->line, i); // replace one word */
	else if (lst_len > 100)
		; // ask for yes no
	else
		; // print all matches (maybe autocomplete equal part)
	return (FUNCT_SUCCESS);
}
