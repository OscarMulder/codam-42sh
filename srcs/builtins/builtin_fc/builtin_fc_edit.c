/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   builtin_fc_edit.c                                  :+:    :+:            */
/*                                                     +:+                    */
/*   By: omulder <omulder@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/09/22 18:54:24 by omulder        #+#    #+#                */
/*   Updated: 2019/10/08 14:14:00 by omulder       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"
#include <stdio.h>

/*
** This function will try to open a tmp file ending with it 0. When it fails
** (most likely because the file exists, it won't open the existing file
** because of O_EXCL) it will increment and try again the number until 99.
** If it still fails at 99 there must be another reason (or 100 editors opened)
** so it will return an error. Since we are not allowed to use errno this is
** a bit hacky.
*/

static int	fc_open_temp(t_fcdata *fc)
{
	int		fd;
	int		i;

	i = 0;
	fc->tmpfile = ft_strjoinfree_s2(FC_TMP_FILE, ft_itoa(i));
	fd = open(fc->tmpfile, (O_CREAT | O_RDWR | O_APPEND | O_EXCL),
		S_IRWXU | S_IRWXG | S_IRWXO);
	while (fd < 0 && i < 100)
	{
		i++;
		ft_strdel(&fc->tmpfile);
		fc->tmpfile = ft_strjoinfree_s2(FC_TMP_FILE, ft_itoa(i));
		fd = open(fc->tmpfile, (O_CREAT | O_RDWR | O_APPEND | O_EXCL),
			S_IRWXU | S_IRWXG | S_IRWXO);
	}
	if (fd < 0)
	{
		ft_eprintf(U_FC);
		g_state->exit_code = EXIT_FAILURE;
		return (FUNCT_FAILURE);
	}
	fc->fd = fd;
	return (FUNCT_SUCCESS);
}

/*
** These flags tell the executor to print the command and to add it to history
*/

static void	set_flags(t_vshdata *data)
{
	data->fc_flags |= FC_PRINT_CMD;
	data->fc_flags |= FC_SET_HIST;
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
	shell_one_line(data, ft_strjoinfree_all(
		ft_strjoinchr(fc->editor, ' '), ft_strjoinchr(fc->tmpfile, '\n')));
	// remove last history entry
	set_flags(data);
	shell_args(data, ft_strdup(fc->tmpfile));
	data->fc_flags = 0;
	// shell execute file etc etc ...
	remove(fc->tmpfile);
}