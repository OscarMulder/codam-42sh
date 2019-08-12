/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   auto_find_matches.c                                :+:    :+:            */
/*                                                     +:+                    */
/*   By: mavan-he <mavan-he@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/08/12 20:20:16 by mavan-he       #+#    #+#                */
/*   Updated: 2019/08/12 21:15:09 by mavan-he      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"

void	auto_lst_del(void *str, size_t size)
{
	(void)size;
	ft_strdel((char**)&str);
}

int		auto_find_matches(t_vshdata *vshdata, char *match, int *i, int state)
{
	t_list	*matchlst;
	int		lst_len;

	(void)i;
	matchlst = NULL;
	if (state == STATE_CMD && auto_get_cmdlst(match, vshdata->envlst,
		&matchlst) == FUNCT_ERROR)
		return (FUNCT_ERROR);
	if (state == STATE_VAR && auto_get_varlst(match, ft_strlen(match),
		vshdata->envlst, &matchlst) == FUNCT_ERROR)
		return (FUNCT_ERROR);
/* 	if (state == STATE_FILE && auto_get_filelst(match, &matchlst) == FUNCT_ERROR)
		return (FUNCT_ERROR); */
	lst_len = ft_lstlen(matchlst);
	ft_putnbr(lst_len);
	while (matchlst != NULL)
	{
		ft_putendl((char *)matchlst->content);
		matchlst = matchlst->next;
	}
	ft_printf("<<<<<<<<< list up, line down >>>>>>>>>\n"); // debug
	if (lst_len == 0)
		return (FUNCT_FAILURE);
	else if (lst_len == 1)
		auto_add_match_toline(match, &vshdata->line, i); // replace one word
	else if (lst_len > 100)
		; // ask for yes no
	else
		; // print all matches (maybe autocomplete equal part)
	ft_lstdel(&matchlst, &auto_lst_del);
	return (FUNCT_SUCCESS);
}