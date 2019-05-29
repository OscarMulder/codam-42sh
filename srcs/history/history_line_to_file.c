/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   history_line_to_file.c                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: mavan-he <mavan-he@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/05/29 15:25:10 by mavan-he       #+#    #+#                */
/*   Updated: 2019/05/29 17:45:10 by mavan-he      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"

int		history_line_to_file(char *line)
{
	int	fd;

	fd = open("/tmp/.vsh_history",
		O_APPEND | O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
	if (fd < 0)
	{
		ft_printf("Cannot open/create vsh history file \n");
		return (FUNCT_ERROR);
	}
	write(fd, line, ft_strlen(line));
	write(fd, "\n", 1);
	close(fd);
	return (FUNCT_SUCCESS);
}
