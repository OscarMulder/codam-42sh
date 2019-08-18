/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   builtin_hash.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: mavan-he <mavan-he@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/08/18 13:09:06 by mavan-he       #+#    #+#                */
/*   Updated: 2019/08/18 14:58:08 by mavan-he      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"

static int	set_flag(char *arg, int *flags)
{
	int i;

	i = 1;
	while (arg[i] != '\0')
	{
		if (arg[i] == 'r')
			*flags |= HASH_LR;
		else
		{
			g_state->exit_code = EXIT_WRONG_USE;
			ft_printf("vsh: hash: -%c: invalid option\n", arg[i]);
			ft_putendl("hash: usage: hash [-r] [utility ...]");
			return (FUNCT_FAILURE);
		}
		i++;
	}
	return (FUNCT_SUCCESS);
}

static int	check_flag(char **args, int *flag, int *argc)
{
	while (args[*argc] != NULL)
	{
		if (args[*argc][0] == '-')
		{
			if (args[*argc][1] == '\0')
				return (FUNCT_SUCCESS);
			if (set_flag(args[*argc], flag) == FUNCT_FAILURE)
				return (FUNCT_FAILURE);
		}
		else
			return (FUNCT_SUCCESS);
		(*argc)++;
	}
	return (FUNCT_SUCCESS);
}

static int	add_to_ht(char *cmd, t_vshdata *vshdata)
{
	int		ret;
	char	*bin_path;

	bin_path = NULL;
	ret = find_binary(cmd, vshdata->envlst, &bin_path);
	if (ret == FUNCT_ERROR)
		return (FUNCT_ERROR);
	if (ret == FUNCT_FAILURE)
	{
		ft_eprintf("vsh: hash: %s: not found\n", cmd);
		return (FUNCT_FAILURE);
	}
	// add bin_path to hash table
}

void		builtin_hash(char **args, t_vshdata *vshdata)
{
	int		flag;
	int		argc;

	(void)vshdata;
	flag = 0;
	argc = 1;
	if (check_flag(args, &flag, &argc) != FUNCT_SUCCESS)
		return ;
	if (flag & HASH_LR)
		; //clear hash table
	while (args[argc] != NULL)
	{
		if (add_to_ht(args[argc], vshdata) == FUNCT_ERROR)
			return ;
		argc++;
	}
}
