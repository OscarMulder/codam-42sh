/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   builtin_export.c                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: jbrinksm <jbrinksm@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/06/05 10:33:08 by jbrinksm       #+#    #+#                */
/*   Updated: 2019/06/07 18:00:13 by codam         ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

/*
** export: usage: export [-n] [name[=value] ...] or export -p
**
** TO DO:
** Read in flags -n -p  (remove from var_extern to var_intern)
** OPTION -p
** List var_extern except if args are given
** OPTION -n
** remove arg keys from var_extern to var_intern (or add new key when it does not exist)
** ARGS:
**	for each arg:
	- If the key is followed by =value, the value of the key is set to value.
	- check if key exists in var_intern (if yes, remove)
	- check if key exists in var_extern (if yes, replace, return)
	- else add key to var_extern

	ERROR:
	export =hoi
bash: export: `=hoi': not a valid identifier
	export -x
bash: export: -x: invalid option
export: usage: export [-n] [name[=value] ...] or export -p

DOne:

** NO ARGS:
**	List var_extern, including empty keys.

??????????? If the -f option is supplied, the names refer to shell functions; otherwise the names refer to shell variables.
*/

#include "vsh.h"

void	builtin_export_noargs(t_envlst *envlst, int flags, int *exit_code)
{
	t_envlst	*probe;

	(void)flags;
	probe = envlst;
	while (probe != NULL)
	{
		#ifdef DEBUG
		if (probe->var == NULL)
		{
			ft_printf("builtin_export: error envlst->var == NULL !!!\n");
			*exit_code = EXIT_FAILURE;
			return ;
		}
		#endif
		ft_putendl(probe->var);
		probe = probe->next;
	}
	*exit_code = EXIT_SUCCESS;
}

/*
**	Not sure if we want to create a new variable if the one to be
**	exported doesn't exist yet. Currently now it will do nothing but
**	set exit_code to EXIT_FAILURE.
*/

void	builtin_change_var_to_type(char *varname, t_envlst *envlst, int *exit_code, int type)
{
	t_envlst	*probe;
	int			varlen;

	probe = envlst;
	varlen = ft_strlen(varname);
	while (probe != NULL)
	{
		if (ft_strncmp(varname, probe->var, varlen) == 0 &&
		probe->var[varlen] == '=')
		{
			probe->type = type;
			return ;
		}
		probe = probe->next;
	}
	*exit_code = EXIT_FAILURE;
}

int		builtin_export_readflags(char *arg, int *flags)
{
	int i;

	if (arg == NULL /* <-- should be redundant */|| arg[0] == '\0')
		return (FUNCT_ERROR);
	i = 1;
	while (arg[i] != '\0')
	{
		if (arg[i] == 'n')
			*flags |= EXP_FLAG_LN;
		else if (arg[i] == 'p')
			*flags |= EXP_FLAG_LP;
		else
		{
			ft_printf("export: invalid option %c\n", arg[i]);
			return (FUNCT_ERROR);
		}
		i++;
	}
	return (FUNCT_SUCCESS);
}

int		builtin_export_getflags(char **args, int *flags, int *argc)
{
	int	i;

	if (args == NULL)
		return (FUNCT_ERROR);
	i = 0;
	while (args[i] != NULL)
	{
		(*argc)++;
		if (ft_strequ(args[i], "--") == true)
			return (FUNCT_SUCCESS);
		if (args[i][0] == '-')
		{
			if (builtin_export_readflags(args[i], flags) == FUNCT_ERROR)
				return (FUNCT_ERROR);
		}
		else
		{
			(*argc)--;
			return (FUNCT_SUCCESS);
		}
		i++;
	}
	return (FUNCT_SUCCESS);
}

void	builtin_export(char **args, t_envlst *envlst, int *exit_code)
{
	int	i;
	int	flags;

	i = 1;
	*exit_code = EXIT_FAILURE;
	if (args == NULL /* should be redundant --> */|| args[0] == NULL)
		return ;
	flags = 0;
	if (builtin_export_getflags(&(args[1]), &flags, &i) == FUNCT_ERROR)
		return ;
	*exit_code = EXIT_SUCCESS;
	if (args[i] == NULL)
		builtin_export_noargs(envlst, flags, exit_code);
	else
		builtin_export_args(&args[i], envlst, exit_code, flags);
}

void	builtin_export_args(char **args, t_envlst *envlst, int *exit_code, int flags)
{
	int i;
	int	type;

	i = 0;
	type = ENV_EXTERN;
	if (flags &= EXP_FLAG_LN)
		type = ENV_LOCAL;
	while (args[i] != NULL)
	{
		builtin_change_var_to_type(args[i], envlst, exit_code, type);
		i++;
	}
}
