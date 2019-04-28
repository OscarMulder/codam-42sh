/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   builtin_echo_set_flags.c                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: mavan-he <mavan-he@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/04/28 11:03:19 by mavan-he       #+#    #+#                */
/*   Updated: 2019/04/28 13:15:30 by mavan-he      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"

static void	echo_merge_flags(char *flags, char arg_flags)
{
	if (arg_flags & OPT_N)
		*flags |= OPT_N;
	if (arg_flags & OPT_E)
	{
		*flags |= OPT_E;
		*flags &= ~OPT_CE;
	}
	if (arg_flags & OPT_CE)
	{
		*flags |= OPT_CE;
		*flags &= ~OPT_E;
	}
}

static int	echo_validate_flag(char *arg_flags, char c)
{
	if (c == 'E')
	{
		*arg_flags |= OPT_CE;
		*arg_flags &= ~OPT_E;
	}
	else if (c == 'e')
	{
		*arg_flags |= OPT_E;
		*arg_flags &= ~OPT_CE;
	}
	else if (c == 'n')
		*arg_flags |= OPT_N;
	else
		return (1);
	return (0);
}

void		echo_set_flags(char **args, char *flags, int *arg_i)
{
	int		i;
	char	arg_flags;

	while (args[*arg_i])
	{
		arg_flags = 0;
		i = 0;
		if (args[*arg_i][0] != '-')
			return ;
		i++;
		while (args[*arg_i][i] != '\0')
		{
			if (echo_validate_flag(&arg_flags, args[*arg_i][i]))
				return ;
			i++;
		}
		echo_merge_flags(flags, arg_flags);
		(*arg_i)++;
	}
}
