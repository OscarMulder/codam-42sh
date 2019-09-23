/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   builtin_fc_edit.c                                  :+:    :+:            */
/*                                                     +:+                    */
/*   By: omulder <omulder@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/09/22 18:54:24 by omulder        #+#    #+#                */
/*   Updated: 2019/09/23 18:01:48 by omulder       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"
#include <stdio.h>

/*
** This function should open a random tmp file, so it doesn't run into issues
** when there are multiple files opened.
*/

static int	fc_open_temp(t_fcdata *fc)
{
	char	*tempfile;
	int		fd;

	tempfile = "/tmp/vsh-fc-tmp123";
	fd = open(tempfile, (O_CREAT | O_RDWR | O_APPEND | O_EXCL));
	if (fd < 0)
	{
		ft_eprintf("vsh: fc: failed to open temporary file\n"); // replace define
		g_state->exit_code = EXIT_FAILURE;
		return (FUNCT_FAILURE);
	}
	fc->fd = fd;
	return (FUNCT_SUCCESS);
}

/*
** So fc_edit has to do a lot of stuff:
** - Find start and end (the commands that need to be edited)
** - Create a temporary file
** - Write the commands to the file
** - Open the file in specified editor/FCEDIT/EDITOR/ed
** - When editor closes
** - Execute the file
** - Remove file
*/

void		fc_edit(t_vshdata *data, t_datahistory *history, t_fcdata *fc)
{
	int		start;
	int		end;

	g_state->exit_code = EXIT_SUCCESS;
	if (fc_get_indexes(history, fc, &start, &end) == FUNCT_FAILURE)
		return ;
	if (fc_open_temp(fc) == FUNCT_FAILURE)
		return ;
	fc_print(history, fc, start, end);
	close(fc->fd);
	data->line->line = ft_strdup("fc \n");
	shell_one_line(data);
	// shell execute file etc etc ...
	remove("/tmp/vsh-fc-tmp123");
}
