/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   input_read.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: omulder <omulder@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/04/17 14:03:16 by rkuijper       #+#    #+#                */
/*   Updated: 2019/09/02 13:58:55 by rkuijper      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"

/*
**	Parse options -p and -l
**	There's also the 'job' parameter which can be present in args, but it doesn't work(?)
**
*/

static int	read_options(char **args, int *arg, int *options)
{
	int i;

	while (*args[*arg] == '-')
	{
		i = 1;
		while (args[*arg][i] != '\0')
		{
			if (args[*arg][i] == 'p')
				*options |= 1;
			else if (args[*arg][i] == 'l')
				*options |= 2;
			else
			{
				ft_eprintf("vsh: bad option: -%c\n", args[*arg][i]);
				return (FUNCT_ERROR);
			}
			i++;
		}
		(*arg)++;
	}
	return (FUNCT_SUCCESS);
}

int			builtin_jobs(char **args, t_vshdata *data)
{
	int	arg;
	int options;

	(void)data;

	arg = 1;
	options = 0;
	if (read_options(args, &arg, &options) != FUNCT_SUCCESS)
		return (FUNCT_ERROR);
	return (FUNCT_SUCCESS);
}
