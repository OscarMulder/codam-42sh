/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   history_get_file_content.c                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: mavan-he <mavan-he@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/05/30 13:49:22 by mavan-he       #+#    #+#                */
/*   Updated: 2019/07/09 12:24:24 by omulder       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"
#include "vsh_history.h"
#include <fcntl.h>
#include "libft.h"
#include <unistd.h>

int		history_get_file_content(void)
{
	int		fd;
	int		ret;
	char	*line;

	history = (char **)ft_memalloc(sizeof(char *) * HISTORY_MAX);
	if (history == NULL)
		return (FUNCT_ERROR);
	fd = open("/tmp/.vsh_history", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
	if (fd == -1)
		return (FUNCT_ERROR);
	ret = 1;
	history_i = 0;
	while (ret > 0 && history_i < HISTORY_MAX - 5)
	{
		line = NULL;
		ret = ft_get_next_line(fd, &line);
		if (ret == -1)
			return (FUNCT_ERROR);
		history[history_i] = line;
		if (ret != 0)
			history_i++;
	}
	close(fd);
	history_tmp = history_i;
	history_cur = history_i;
	return (FUNCT_SUCCESS);
}
