/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   history_get_file_content.c                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: mavan-he <mavan-he@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/05/30 13:49:22 by mavan-he       #+#    #+#                */
/*   Updated: 2019/07/15 16:40:14 by omulder       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"
#include "vsh_history.h"
#include <fcntl.h>
#include "libft.h"
#include <unistd.h>

/*
** Get the content out of the history file
*/

int		history_get_file_content(t_history ***history)
{
	int		fd;
	int		ret;
	int		i;
	char	*line;

	*history = (t_history **)ft_memalloc(sizeof(t_history *) * HISTORY_MAX);
	if (*history == NULL)
		return (FUNCT_ERROR);
	fd = open(HISTFILE, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
	if (fd == -1)
		return (FUNCT_ERROR);
	ret = 1;
	i = 0;
	while (ret > 0 && i < HISTORY_MAX)
	{
		line = NULL;
		ret = ft_get_next_line(fd, &line);
		if (ret == -1)
			return (FUNCT_ERROR);
		(*history)[i]->number = i;
		(*history)[i]->str = line;
		i++;
	}
	close(fd);
	return (FUNCT_SUCCESS);
}
