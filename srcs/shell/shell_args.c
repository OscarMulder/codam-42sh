/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   shell_args.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: mavan-he <mavan-he@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/09/15 14:41:31 by mavan-he       #+#    #+#                */
/*   Updated: 2019/09/22 16:22:51 by mavan-he      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"

static int	shell_check_file(char *filepath)
{
	int ret;

	if (access(filepath, F_OK) != 0)
	{
		ft_eprintf(E_NO_SUCH_P, filepath);
		return (err_ret_exit(NULL, EXIT_NOTFOUND));
	}
	if (access(filepath, R_OK) != 0)
	{
		ft_eprintf(E_N_PER_DEN, filepath);
		return (err_ret_exit(NULL, EXIT_NOT_EXECUTABLE));
	}
	ret = ft_is_regular_file(filepath);
	if (ret == FUNCT_ERROR)
	{
		ft_eprintf(E_STAT_P, filepath);
		return (err_ret_exit(NULL, EXIT_FAILURE));
	}
	return (ret);
}

/*
**	shell_args gets called when argc > 1 
**	shell_check_file will check if arg 1 is a valid file 
**	shell_init_line will read the input from file into line
**	shell_lines_exec will split the file input at line into cmd lines
**	and execute each cmd line one by one 
*/

void		shell_args(t_vshdata *data, char *filepath)
{
	char **lines;

	if (shell_get_path(data, &filepath) == FUNCT_ERROR)
		return ;
	if (shell_check_file(filepath) != FUNCT_SUCCESS)
		return ;
	if (shell_init_line(data, filepath) == FUNCT_FAILURE)
		return ;
	ft_strdel(&filepath);
	lines = shell_line_splitter(data);
	if (lines == NULL)
		return ;
	shell_lines_exec(data, lines);
	ft_strarrdel(&lines);
	return ;
}
