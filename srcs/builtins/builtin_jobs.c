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

#define JOB_OPT_NONE	0
#define JOB_OPT_P		1
#define JOB_OPT_L		2

static int	read_options(char **args, int *arg, int *options)
{
	int i;

	while (args[*arg] != NULL && *args[*arg] == '-')
	{
		i = 1;
		while (args[*arg][i] != '\0')
		{
			if (args[*arg][i] == 'p')
				*options |= JOB_OPT_P;
			else if (args[*arg][i] == 'l')
				*options |= JOB_OPT_L;
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
	options = JOB_OPT_NONE;
	/* Try to read out all options. */
	if (read_options(args, &arg, &options) != FUNCT_SUCCESS)
		return (FUNCT_ERROR);
	if (args[arg] != NULL) /* Start parsing the rest of the parameters. */
	{

	}
	else /* No parameters left after options, start logging ALL jobs. */
	{
		
	}	
	return (FUNCT_SUCCESS);
}
