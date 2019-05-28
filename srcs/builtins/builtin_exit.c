/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   builtin_exit.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: jbrinksm <jbrinksm@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/04/11 20:15:24 by jbrinksm       #+#    #+#                */
/*   Updated: 2019/05/28 18:50:53 by omulder       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"

void	builtin_exit(char exitcode)
{
	ft_printf("exit\n");
	exit((int)exitcode);
}
