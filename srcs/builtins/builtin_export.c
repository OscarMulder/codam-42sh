/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   builtin_export.c                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: jbrinksm <jbrinksm@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/06/05 10:33:08 by jbrinksm       #+#    #+#                */
/*   Updated: 2019/07/07 22:37:35 by jbrinksm      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

/*
**	export: usage: export [-n] [name[=value] ...] or export -p
**
**	TO DO:
**	- We currently do not add quotes for -p, this is because of our
**	wrong envlst structure, we need to seperate value from varname.
**	OPTION -n
**	remove arg keys from var_extern to var_intern (or add new key when it does not exist)
**	ARGS:
**	for each arg:
**	- If the key is followed by =value, the value of the key is set to value.
**	- check if key exists in var_intern (if yes, remove)
**	- check if key exists in var_extern (if yes, replace, return)
**	- else add key to var_extern
**
**	DOne:
**
**	- Read in flags -n -p  (remove from var_extern to var_intern)
**	- print usage on invalid option.
**	- checks for valid identifier. Will print error for every invalid identifier and
**  any valid identifier will still be handled.
**	- checks for valid options. (will exit on invalid option)
**	- OPTION -p
**		- List var_extern except if args are given
**	- NO ARGS:
**		- List var_extern, including empty keys.

??????????? If the -f option is supplied, the names refer to shell functions; otherwise the names refer to shell variables.
*/

#include "vsh.h"

void	builtin_export_print(t_envlst *envlst, int flags, int *exit_code)
{
	t_envlst	*probe;

	probe = envlst;
	while (probe != NULL)
	{
		#ifdef DEBUG
		if (probe->var == NULL)
		{
			ft_putendl("builtin_export: error envlst->var == NULL !!!");
			*exit_code = EXIT_FAILURE;
			return ;
		}
		#endif
		if (flags & EXP_FLAG_LP)
			ft_putstr("declare -x ");
		ft_putendl(probe->var);
		probe = probe->next;
	}
}

void	builtin_export_var_to_type(char *varname, t_envlst *envlst, int *exit_code, int type)
{
	t_envlst	*probe;
	int			varlen;

	probe = envlst;
	varlen = ft_strlen(varname);
	while (probe != NULL)
	{
		if (ft_strnequ(varname, probe->var, varlen) == true &&
		probe->var[varlen] == '=')
		{
			probe->type = type;
			return ;
		}
		probe = probe->next;
	}
	builtin_assign(varname, envlst, exit_code); /* should add option to change assign type */
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
			ft_printf("vsh: export: -%c: invalid option\n", arg[i]);
			ft_putendl("export: usage: export [-n] [name[=value] ...] or export -p");
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

void	builtin_export_args(char **args, t_envlst *envlst, int *exit_code, int flags)
{
	int i;
	int	type;

	i = 0;
	type = ENV_EXTERN;
	if (flags & EXP_FLAG_LN)
		type = ENV_LOCAL;
	while (args[i] != NULL)
	{
		/* The arg needs to be checked and split if it is not just identifier,
		also in the above functions the names like "varname" are wrong */
		if (tools_is_valid_identifier(args[i]) == true)
			builtin_export_var_to_type(args[i], envlst, exit_code, type);
		/*
		else if it has =value after it, set identifier to value
		*/
		else
		{
			*exit_code = EXIT_FAILURE;
			ft_printf("vsh: export: '%s': not a valid identifier\n", args[i]);
		}
		i++;
	}
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
		builtin_export_print(envlst, flags, exit_code);
	else
		builtin_export_args(&args[i], envlst, exit_code, flags);
}
