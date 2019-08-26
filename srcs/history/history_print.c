/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   history_print.c                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: mavan-he <mavan-he@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/05/30 20:47:41 by mavan-he       #+#    #+#                */
/*   Updated: 2019/08/26 15:18:19 by omulder       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"
#include "libft.h"

/*
** Print the history
*/

static char	*add_tabs(char *str, char *new)
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
	return (new);
}

static char	*add_tabs_after_newlines(char *str)
{
	int		i;
	int		count;
	char	*new;

	i = 0;
	count = 0;
	while (str[i] != '\0')
	{
		if (str[i] == '\n' && str[i + 1] != '\0')
			count++;
		i++;
	}
	new = ft_strnew(i + count);
	if (new == NULL)
		return (NULL);
	return (add_tabs(str, new));
}

static void	find_start(t_history **history, int *smallest, int *start)
{
	int i;

	i = 0;
	*smallest = HISTORY_MAX + 1;
	while (i < HISTORY_MAX && history[i]->str != NULL)
	{
		if (history[i]->number < *smallest)
		{
			*start = i;
			*smallest = history[i]->number;
		}
		i++;
	}
}

void		history_print(t_history **history)
{
	int		i;
	int		smallest;
	int		start;
	char	*tmp;

	find_start(history, &smallest, &start);
	i = start;
	while (i < HISTORY_MAX && history[i]->str != NULL)
	{
		tmp = add_tabs_after_newlines(history[i]->str);
		ft_printf("%d\t%s\n", history[i]->number, tmp);
		ft_strdel(&tmp);
		i++;
	}
	i = 0;
	while (start != 0 && i < start && history[i]->str != NULL)
	{
		tmp = add_tabs_after_newlines(history[i]->str);
		ft_printf("%d\t%s\n", history[i]->number, tmp);
		ft_strdel(&tmp);
		i++;
	}
}
