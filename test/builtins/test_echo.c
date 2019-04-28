/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   test_echo.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: mavan-he <mavan-he@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/04/28 16:14:29 by mavan-he       #+#    #+#                */
/*   Updated: 2019/04/28 17:17:33 by mavan-he      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"

int		test_echo(void)
{
	char *args[4];

	args[0] = "echo";
	args[1] = "-neE";
	args[2] = "\\\\abc\t\v\r\f\n";
	args[3] = NULL;
	builtin_echo(args);
	return (FUNCT_SUCCESS);
}
