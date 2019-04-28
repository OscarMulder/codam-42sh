/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   test_echo.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: mavan-he <mavan-he@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/04/28 16:14:29 by mavan-he       #+#    #+#                */
/*   Updated: 2019/04/28 17:42:58 by mavan-he      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"

int		test_echo(void)
{
	char *args[4];

	args[0] = "echo";
	args[1] = "-neE";
	args[2] = "\\\\abc\t\v\r\f";
	args[3] = NULL;
	builtin_echo(args);
	args[0] = "echo";
	args[1] = NULL;
	builtin_echo(args);
	args[0] = "echo";
	args[1] = "-nE";
	args[2] = "\n";
	args[3] = NULL;
	builtin_echo(args);
	args[0] = "echo";
	args[1] = "-nE";
	args[2] = NULL;
	builtin_echo(args);
	args[0] = "echo";
	args[1] = "-nEa";
	args[2] = NULL;
	builtin_echo(args);
	return (FUNCT_SUCCESS);
}
