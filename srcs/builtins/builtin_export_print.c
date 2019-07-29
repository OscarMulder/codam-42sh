/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   builtin_export_print.c                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: mavan-he <mavan-he@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/07/19 12:13:35 by mavan-he       #+#    #+#                */
/*   Updated: 2019/07/29 11:22:31 by mavan-he      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"

static bool	escape_char(char c)
{
	return (c == '$' || c == '\"' || c == '\\');
}

static void	builtin_export_escape_print(char *var)
{
	char		*value;
	int			i;

	value = ft_strchr(var, '=');
	if (value != NULL)
	{
		i = 1;
		ft_printf("%.*s\"", value - var + 1, var);
		while (value[i] != '\0')
		{
			if (escape_char(value[i]) == true)
				ft_putchar('\\');
			ft_putchar(value[i]);
			i++;
		}
		ft_putstr("\"\n");
	}
	else
		ft_putendl(var);

}

void		builtin_export_print(t_envlst *envlst, int flags)
{
	t_envlst	*probe;

	probe = envlst;
	while (probe != NULL)
	{
		if (probe->type & ENV_EXTERN)
		{
			if (flags & EXP_FLAG_LP)
				ft_putstr("declare -x ");
			builtin_export_escape_print(probe->var);
		}
		probe = probe->next;
	}
}
