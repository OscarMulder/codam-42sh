/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   history_to_file.c                                  :+:    :+:            */
/*                                                     +:+                    */
/*   By: omulder <omulder@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/05/29 15:25:10 by mavan-he       #+#    #+#                */
/*   Updated: 2019/09/21 22:02:14 by omulder       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"
#include "libft.h"
#include <fcntl.h>
#include <unistd.h>
#include <limits.h>

/*
** Write the history to file
*/

static void	find_start(t_history **history, int *smallest, int *start)
{
	int i;

	i = 0;
	*smallest = INT_MAX;
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

static void	history_print_to_fd(int fd, t_history **history)
{
	int		i;
	int		smallest;
	int		start;

	find_start(history, &smallest, &start);
	i = start;
	while (i < HISTORY_MAX && history[i]->str != NULL)
	{
		ft_dprintf(fd, "%s%c", history[i]->str, HIST_SEPARATE);
		i++;
	}
	i = 0;
	while (start != 0 && i < start && history[i]->str != NULL)
	{
		ft_dprintf(fd, "%s%c", history[i]->str, HIST_SEPARATE);
		i++;
	}
}

int			history_to_file(t_vshdata *data)
{
	int		fd;

	if (data->history->history == NULL || data->history->history_file == NULL)
		return (FUNCT_ERROR);
	fd = open(data->history->history_file,
	O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
	if (fd == -1)
	{
		ft_eprintf(E_HIST_OPEN_STR);
		return (FUNCT_ERROR);
	}
	history_print_to_fd(fd, data->history->history);
	close(fd);
	return (FUNCT_SUCCESS);}
