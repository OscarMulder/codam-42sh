/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   builtin_echo.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: jbrinksm <jbrinksm@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/03/28 10:21:20 by jbrinksm       #+#    #+#                */
/*   Updated: 2019/04/28 17:15:21 by mavan-he      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

/*
** WORK IN PROGRESS
** Not all escape characters have been added
** how far do we want to go ?
** builtin_echo can receive options in env ?
*/

#include "vsh.h"

static int	echo_isspecial(char c)
{
	if (c == 't')
		return (FUNCT_SUCCESS);
	if (c == 'n')
		return (FUNCT_SUCCESS);
	if (c == 'v')
		return (FUNCT_SUCCESS);
	if (c == 'f')
		return (FUNCT_SUCCESS);
	if (c == 'r')
		return (FUNCT_SUCCESS);
	if (c == '\\')
		return (FUNCT_SUCCESS);
	return (FUNCT_FAILURE);
}

static int	echo_replacespecial(char c)
{
	if (c == 't')
		return ('\t');
	if (c == 'n')
		return ('\n');
	if (c == 'v')
		return ('\v');
	if (c == 'f')
		return ('\f');
	if (c == 'r')
		return ('\r');
	if (c == '\\')
		return ('\\');
	return (0);
}

static void	echo_escape_chars(char *arg)
{
	int i;
	int i_new;

	i = 0;
	i_new = 0;
	while (arg[i] != '\0')
	{
		if (arg[i] == '\\' && echo_isspecial(arg[i + 1]))
		{
			i++;
			arg[i_new] = echo_replacespecial(arg[i]);
		}
		else
			arg[i_new] = arg[i];
		i_new++;
		i++;
	}
	while (arg[i_new] != '\0')
	{
		arg[i_new] = '\0';
		i_new++;
	}
}

/*
** if option xpg_echo is set, turn on -e, need env for this ?
*/

int			builtin_echo(char **args)
{
	int		arg_i;
	char	flags;

	arg_i = 1;
	flags = echo_set_flags(args, &arg_i);
	while (args[arg_i])
	{
		if (flags & OPT_E)
			echo_escape_chars(args[arg_i]);
		ft_putstr(args[arg_i]);
		if (args[arg_i + 1] != NULL)
			ft_putchar(' ');
		arg_i++;
	}
	if ((flags & OPT_N) == 0)
		ft_putchar('\n');
	return (FUNCT_SUCCESS);
}
