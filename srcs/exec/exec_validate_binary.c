/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   exec_validate_binary.c                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: mavan-he <mavan-he@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/08/03 17:21:53 by mavan-he       #+#    #+#                */
/*   Updated: 2019/08/03 17:42:36 by mavan-he      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"

int		exec_validate_binary(char *binary)
{
	if (access(binary, F_OK) == -1)
	{
		ft_eprintf("vsh: no such file or directory: %s\n", binary);
		return (err_ret_exit(NULL, EXIT_NOTFOUND));
	}
	else if (access(binary, X_OK) == -1)
	{
		ft_eprintf("vsh: %s: Permission denied\n", binary);
		return (err_ret_exit(NULL, EXIT_NOT_EXECUTABLE));
	}
	else
		return (FUNCT_SUCCESS);
}
