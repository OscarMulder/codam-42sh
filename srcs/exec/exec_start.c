/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   exec_start.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: omulder <omulder@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/05/29 17:52:22 by omulder        #+#    #+#                */
/*   Updated: 2019/05/31 10:20:28 by tde-jong      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"

static char	**init_array(t_ast *ast)
{
	char	**args;

	if (ast == NULL)
		return (NULL);
	args = (char**)ft_memalloc(sizeof(char*) * 2);
	if (args == NULL)
		return (NULL);
	args[0] = ft_strdup(ast->value);
	if (args[0] == NULL)
	{
		ft_freestrarray(&args);
		return (NULL);
	}
	return (args);
}

static int	add_argument(char ***args, char *value)
{
	char	*temp;

	temp = ft_strdup(value);
	if (temp == NULL)
	{
		ft_freestrarray(args);
		return (FUNCT_FAILURE);
	}
	if (ft_strarradd(args, temp) == FUNCT_FAILURE)
	{
		ft_strdel(&temp);
		ft_freestrarray(args);
		return (FUNCT_FAILURE);
	}
	ft_strdel(&temp);
	return (FUNCT_SUCCESS);
}

static char	**create_args(t_ast *ast)
{
	char	**args;
	t_ast	*probe;

	args = (init_array(ast));
	if (args == NULL)
		return (NULL);
	probe = ast->child;
	while (probe)
	{
		if (add_argument(&args, probe->value) == FUNCT_FAILURE)
			return (NULL);
		probe = probe->child;
	}
	return (args);
}

int			exec_start(t_ast *ast, int *exit_code)
{
	char	**env;
	char	**args;

	env = env_get_environ_cpy();
	if (env == NULL)
		return (FUNCT_FAILURE);
	if (ast->type == WORD)
	{
		args = create_args(ast);
		if (args == NULL)
		{
			ft_freestrarray(&env);
			return (FUNCT_FAILURE);
		}
		return (exec_cmd(args, &env, exit_code));
	}
	else
	{
		ft_printf("N0p3: I only do the easy shit, don't fuckup!\n");
		return (FUNCT_FAILURE);
	}
}
