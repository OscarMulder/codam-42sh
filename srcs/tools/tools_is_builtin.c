/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   tools_is_builtin.c                                 :+:    :+:            */
/*                                                     +:+                    */
/*   By: jbrinksm <jbrinksm@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/07/20 21:28:30 by jbrinksm       #+#    #+#                */
/*   Updated: 2019/10/01 15:18:27 by omulder       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"

bool	tools_is_builtin(char *exec_name)
{
	if (exec_name == NULL)
		return (false);
	if (ft_strequ(exec_name, "echo") || ft_strequ(exec_name, "exit") ||
		ft_strequ(exec_name, "cd") || ft_strequ(exec_name, "export") ||
		ft_strequ(exec_name, "set") || ft_strequ(exec_name, "unset") ||
		ft_strequ(exec_name, "history") || ft_strequ(exec_name, "type") ||
		ft_strequ(exec_name, "alias") || ft_strequ(exec_name, "unalias")
		|| ft_strequ(exec_name, "hash") || ft_strequ(exec_name, "fc") ||
		ft_strequ(exec_name, "setenv") || ft_strequ(exec_name, "env") ||
		ft_strequ(exec_name, "unsetenv"))
		return (true);
	return (false);
}
