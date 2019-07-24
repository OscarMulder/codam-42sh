/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   builtin_alias.c                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: mavan-he <mavan-he@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/07/24 13:36:44 by mavan-he       #+#    #+#                */
/*   Updated: 2019/07/24 18:29:49 by mavan-he      ########   odam.nl         */
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

static int	builtin_alias_print(char *arg, t_aliaslst *aliaslst)
{
	char	*str_equal;
	int		arg_len;

	arg_len = ft_strlen(arg);
	while (aliaslst != NULL)
	{
		if (ft_strnequ(arg, aliaslst->var, arg_len) == true &&
			(aliaslst->var[arg_len] == '='))
		{
			str_equal = ft_strchr(aliaslst->var, '=');
			ft_printf("%.*s\'%s\'\n", str_equal - aliaslst->var + 1,
			aliaslst->var, str_equal + 1);
			return (FUNCT_SUCCESS);
		}	
		aliaslst = aliaslst->next;
	}
	ft_eprintf("vsh: alias: %s: not found\n", arg);
	return (FUNCT_FAILURE);
}

static int	builtin_alias_args(char **args, int i, t_aliaslst **aliaslst)
{
	int ret;

	ret = FUNCT_SUCCESS;
	while (args[i] != NULL)
	{
		if (ft_strchr(args[i], '=') != NULL)
		{
			if (builtin_alias_set(args[i], aliaslst) == FUNCT_ERROR)
				ret = FUNCT_ERROR;
		}
		else if (builtin_alias_print(args[i], *aliaslst) == FUNCT_ERROR)
			ret = FUNCT_ERROR;
		i++;
	}
	return (ret);
}

static void	builtin_alias_printlst(t_aliaslst *aliaslst)
{
	char	*str_equal;

	while (aliaslst != NULL)
	{
		str_equal = ft_strchr(aliaslst->var, '=');
		ft_printf("%.*s\'%s\'\n", str_equal - aliaslst->var + 1,
		aliaslst->var, str_equal + 1);
		aliaslst = aliaslst->next;
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

void		builtin_alias(char **args, t_aliaslst **aliaslst)
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
	if (args[i] == NULL || flag & ALIAS_FLAG_LP)
		builtin_alias_printlst(*aliaslst);
	if (args[i] != NULL && builtin_alias_args(args, i, aliaslst) == FUNCT_ERROR)
		;// set global error to EXIT_ERROR
	else
		;// set global error to EXIT_ERROR
}
