/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   history_to_file.c                                  :+:    :+:            */
/*                                                     +:+                    */
/*   By: mavan-he <mavan-he@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/05/29 15:25:10 by mavan-he       #+#    #+#                */
/*   Updated: 2019/07/16 17:02:18 by omulder       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"
#include "vsh_history.h"
#include "libft.h"
#include <fcntl.h>
#include <unistd.h>

/*
** Write the history to file
*/

int		history_to_file(t_history **history)
{
	int		fd;
	int		i;

	fd = open("/users/omulder/.vsh_history", O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
	if (fd == -1)
	{
		ft_putstr_fd("Cannot open/create vsh history file \n", STDERR_FILENO);
		return (FUNCT_ERROR);
	}
	i = 0;
	while (history[i]->str != NULL)
	{
		ft_dprintf(fd, "%s%c", history[i]->str, '\n');
		i++;
	}
	close(fd);
	return (FUNCT_SUCCESS);
}
