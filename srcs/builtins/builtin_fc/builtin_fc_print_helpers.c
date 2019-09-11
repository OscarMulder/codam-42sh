/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   builtin_fc_print_helpers.c                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: omulder <omulder@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/09/11 13:03:14 by omulder        #+#    #+#                */
/*   Updated: 2019/09/11 13:40:11 by omulder       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"

static int	add_tabs(char *str, char *new)
{
	int		i;
	int		j;

	i = 0;
	j = 0;
	while (str[i] != '\0')
	{
		new[j] = str[i];
		if (str[i] == '\n' && str[i + 1] != '\0')
		{
			j++;
			new[j] = '\t';
		}
		i++;
		j++;
	}
	return (true);
}

static int	add_tabs_after_newlines(char *str, char **new)
{
	int		i;
	int		tab_count;

	i = 0;
	tab_count = 0;
	while (str[i] != '\0')
	{
		if (str[i] == '\n' && str[i + 1] != '\0')
			tab_count++;
		i++;
	}
	if (tab_count == 0)
	{
		*new = str;
		return (false);
	}
	*new = ft_strnew(i + tab_count);
	if (*new == NULL)
		return (false);
	return (add_tabs(str, *new));
}

void		fc_list_print_line(t_history *history, t_fcdata *fc)
{
	int		ret;
	char	*tmp;

	ret = add_tabs_after_newlines(history->str, &tmp);
	if (fc->options & FC_OPT_N)
		ft_printf("\t%s\n", tmp);
	else
		ft_printf("%d\t%s\n", history->number, tmp);
	if (ret == true)
		ft_strdel(&tmp);
}
