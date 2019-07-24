/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   builtin_alias.c                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: mavan-he <mavan-he@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/07/24 13:36:44 by mavan-he       #+#    #+#                */
/*   Updated: 2019/07/24 17:02:40 by mavan-he      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"

// alias [-p] [name[=value] ...]
// 
// The name cannot contain any special shell characters
// When -p is set, alias prints all aliases, it also handles arguments
// 

// Alias substitudes the first WORD in a simple command with it's value
// If the first WORD contains a special shell character it is skipped

static int	builtin_alias_args(char **args, int i, t_iets aliaslst)
{
	
}
static void	builtin_alias_print(t_iets **aliaslst)
{
	char	*str_equal;

	if (*aliaslst == NULL)
		return ;
	while (*aliaslst != NULL)
	{
		str_equal = ft_strchr(aliaslst->var, '=');
		ft_printf("%.*s\'%s\'\n", str_equal - aliaslst->var + 1,
		aliaslst->var, str_equal + 1);
		aliaslst++;
	}
}

static int	builtin_alias_flag(char **args, int *flag, int *i)
{
	int j;

	while (args[*i] != NULL)
	{
		j = 1;
		if (args[*i][0] != '-' || args[*i][1] == '\0')
			return (FUNCT_SUCCESS);
		while (args[*i][j] != '\0')
		{
			if (args[*i][j] != 'p')
			{
				ft_eprintf("vsh: alias: -%c: invalid option\n", args[*i][j]);
				ft_eprintf("alias: usage: alias [-p] [name[=value] ... ]\n");
				return (FUNCT_FAILURE);
			}
			(*flag) |= ALIAS_FLAG_LP;
			j++;
		}
		(*i)++;
	}
	return (FUNCT_SUCCESS);
}

void		builtin_alias(char **args, t_iets **aliaslst)
{
	int		flag;
	int		i;

	i = 1;
	flag = 0;
	if (builtin_alias_flag(args, &flag, &i) == FUNCT_FAILURE)
	{
		// set global error;
		return ;
	}
	if (flag & ALIAS_FLAG_LP)
		builtin_alias_print(aliaslst);
	if (args[i] != NULL && builtin_alias_args(args, i, aliaslst) == FUNCT_ERROR)
		;// set global error to EXIT_ERROR
	else
		;// set global error to EXIT_ERROR
}
