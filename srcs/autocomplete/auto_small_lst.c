/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   auto_small_lst.c                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: mavan-he <mavan-he@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/08/13 17:40:10 by mavan-he       #+#    #+#                */
/*   Updated: 2019/08/16 12:18:18 by omulder       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"

int		auto_compare(char *str1, char *str2)
{
	int i;

	i = 0;
	while (str1[i] != '\0' && str1[i] == str2[i])
		i++;
	return (i);
}

int		auto_anything_to_add(int i, t_list *matchlst, char **to_add)
{
	char	*match;
	int		match_len;
	int		compare_len;

	match = (char *)matchlst->content;
	match_len = ft_strlen(&match[i]);
	while (matchlst != NULL)
	{
		compare_len = auto_compare(&match[i], &((char *)matchlst->content)[i]);
		if (compare_len < match_len)
			match_len = compare_len;
		if (match_len == 0)
			break ;
		matchlst = matchlst->next;
	}
	if (match_len > 0)
		*to_add = ft_strndup(match, match_len + i);
	else
		return (FUNCT_FAILURE);
	if (*to_add == NULL)
	{
		ft_eprintf(E_ALLOC_STR);
		return (FUNCT_ERROR);
	}
	return (FUNCT_SUCCESS);
}

int		auto_small_lst(char *match, t_list *matchlst,
		t_vshdata *vshdata, t_inputdata *data)
{
	char *to_add;

	to_add = NULL;
	if (auto_anything_to_add(ft_strlen(match), matchlst, &to_add)
		== FUNCT_ERROR)
		return (FUNCT_ERROR);
	if (to_add != NULL)
		return (auto_add_match_toline(match, to_add, vshdata,
		data));
	else
	{
		ft_putchar('\n');
		auto_lst_print(matchlst, auto_lst_count(matchlst));
	}
	return (FUNCT_SUCCESS);
}
