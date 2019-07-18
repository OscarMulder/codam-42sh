/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   builtin_export.c                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: jbrinksm <jbrinksm@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/06/05 10:33:08 by jbrinksm       #+#    #+#                */
/*   Updated: 2019/07/18 17:44:50 by mavan-he      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

/*
**	export: usage: export [-n] [name[=value] ...] or export -p
**
**	TO DO:
**
**	Fix bug with export hoi=doei ; export hoi=nudit ; export
**
**	Done:
**
**	- Read in flags -n -p  (remove from var_extern to var_intern)
**	- print usage on invalid option.
**	- checks for valid identifier. Print error for every invalid identifier
**	- OPTION -p
**		- List var_extern except if args are given
**	- OPTION -n
**		- remove arg keys from var_extern to var_intern 
**		(or add new key when it does not exist)
**	- NO ARGS:
**		- List var_extern, including empty keys and value surrounded by quotes
*/

#include "vsh.h"

void	builtin_export_print(t_envlst *envlst, int flags, int *exit_code)
{
	t_envlst	*probe;
	char		*value;

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
		if (probe->type == ENV_EXTERN)
		{
			if (flags & EXP_FLAG_LP)
				ft_putstr("declare -x ");
			value =	ft_strchr(probe->var, '=');
			if (value != NULL)
			{
				ft_putsubstr(probe->var, 0, value - probe->var + 1);
				ft_putchar('"');
				ft_putstr(value + 1);
				ft_putstr("\"\n");
			}
			else
				ft_putendl(probe->var);
		}
		probe = probe->next;
	}
}

/* static bool	tools_is_assignment(char *arg)
{
	int i;

	if (arg == NULL || *arg == '=')
		return (false);
	i = 0;
	while (arg[i] != '\0')
	{
		if (arg[i] == '=')
			return (true);
		i++;
	}
	return (false);
}

static char	*tools_get_identifier(char *envstr)
{
	int		len;
	char	*identifier;

	len = ft_strclen(envstr, '=');
	if (len == 0)
		return (NULL);
	identifier = ft_strcdup(envstr, '=');
	return (identifier);
} */

/* void	builtin_export_var_to_type(char *arg, t_envlst *envlst, int *exit_code, int type)
{
	t_envlst	*probe;
	int			varlen;
	char		*identifier;

	probe = envlst;
	varlen = ft_strlen(arg);
	identifier = arg;
	if (tools_is_assignment(arg) == true)
		identifier = tools_get_identifier(arg);
	if (identifier == NULL)
	{
		*exit_code = EXIT_FAILURE;
		return ;
	}
	while (probe != NULL)
	{
		if (ft_strnequ(identifier, probe->var, varlen) == true &&
		probe->var[varlen] == '=')
		{
			probe->type = type;
			return ;
		}
		probe = probe->next;
	}
	builtin_assign(arg, envlst, exit_code, type);
} */

static void	builtin_export_arg(char *arg, t_envlst *envlst, int *exit_code, int type)
{
	t_envlst	*probe;
	int			arglen;
	
	probe = envlst;
	arglen = ft_strlen(arg);
	if (ft_strchr(arg, '=') == NULL)
	{
		while (probe != NULL)
		{
			if (ft_strnequ(arg, probe->var, arglen) == true &&
			(probe->var[arglen] == '=' || probe->var[arglen] == '\0'))
			{
				probe->type = type;
				return ;
			}
			probe = probe->next;
		}
		builtin_assign(arg, envlst, exit_code, type);
	}
	else
		builtin_assign(arg, envlst, exit_code, type);
}

int			builtin_export_readflags(char *arg, int *flags)
{
	int i;

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
		if (tools_is_valid_identifier(args[i]) == true)
			builtin_export_arg(args[i], envlst, exit_code, type);
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
	if (args == NULL)
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
