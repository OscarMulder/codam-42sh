/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   history_line_to_array.c                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: omulder <omulder@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/05/30 18:55:25 by mavan-he       #+#    #+#                */
/*   Updated: 2019/09/26 11:43:41 by tde-jong      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"
#include "libft.h"

/*
** Add one line to the history array
*/

static void	history_find_start(t_history **history, int *number, int *start)
{
	int i;
	int	smallest;

	i = 0;
	smallest = history[i]->number;
	while (i < HISTORY_MAX && history[i]->str != NULL)
		i++;
	if (i != HISTORY_MAX)
	{
		*start = i;
		*number = i;
		return ;
	}
	i = 0;
	while (i < HISTORY_MAX)
	{
		if (history[i]->number <= smallest)
		{
			*start = i;
			smallest = history[i]->number;
		}
		if (history[i]->number > *number)
			*number = history[i]->number;
		i++;
	}
}

static int	is_same_cmd(t_history **history, char *line, int index,
						int line_len)
{
	int prev;

	if (index > 0)
		prev = index - 1;
	else
		prev = HISTORY_MAX - 1;
	if (history[prev]->str == NULL)
		return (false);
	if (line[line_len - 1] == '\n')
		line_len--;
	if (ft_strnequ(history[prev]->str, line, line_len) == true)
		return (true);
	return (false);
}

int			history_line_to_array(t_history **history, char **line)
{
	int start;
	int number;
	int	line_len;

	line_len = ft_strlen(*line);
	if (line_len < 1 || (line_len == 1 && (*line)[0] == '\n'))
		return (FUNCT_SUCCESS);
	number = -1;
	start = 0;
	history_find_start(history, &number, &start);
	if (is_same_cmd(history, *line, start, line_len) == true)
		return (FUNCT_SUCCESS);
	if (history[start]->str != NULL)
		ft_strdel(&history[start]->str);
	history[start]->number = number + 1;
	if ((*line)[line_len - 1] == '\n')
		line_len--;
	history[start]->str = ft_strsub(*line, 0, line_len);
	if (history[start]->str == NULL)
	{
		ft_strdel(line);
		ft_eprintf(E_N_ALLOC_STR, "history");
		return (FUNCT_ERROR);
	}
	return (FUNCT_SUCCESS);
}
