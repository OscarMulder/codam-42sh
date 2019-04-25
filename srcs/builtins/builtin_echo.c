/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   builtin_echo.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: jbrinksm <jbrinksm@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/03/28 10:21:20 by jbrinksm       #+#    #+#                */
/*   Updated: 2019/04/25 17:34:19 by mavan-he      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

/*
** WORK IN PROGRESS
*/

#include "vsh.h"

int			builtin_echo(char **args)
{
	size_t i;

	i = 0;
	while (args[i])
	{
		if (i > 0)
			ft_putchar(' ');
		ft_putstr(args[i]);
		i++;
	}
	ft_putchar('\n');
	return (FUNCT_SUCCESS);
}

/* t_echo_flags	*vsh_echo_getflags(char *arg)
{
	t_echo_flags	*new;

	new = (t_echo_flags*)malloc(sizeof(t_echo_flags));
	ft_malloc_checker(new);
	new->flag_found = 1;
	new->n_flag = 0;
	new->e_flag = 0;
	new->cap_e_flag = 0;
	if (arg && !ft_strcmp(arg, "-n"))
		new->n_flag = 1;
	else if (arg && !ft_strcmp(arg, "-e"))
		new->e_flag = 1;
	else if (arg && !ft_strcmp(arg, "-E"))
		new->cap_e_flag = 1;
	else
		new->flag_found = 0;
	return (new);
}

void			vsh_echo_replacespecial(char *c)
{
	if (*c == 't')
		*c = '\t';
	if (*c == 'n')
		*c = '\n';
	if (*c == 'v')
		*c = '\v';
	if (*c == 'f')
		*c = '\f';
	if (*c == 'r')
		*c = '\r';
}

int				vsh_echo_isspecial(char c)
{
	if (c == 't')
		return (FUNCT_SUCCESS);
	if (c == 'n')
		return (FUNCT_SUCCESS);
	if (c == 'v')
		return (FUNCT_SUCCESS);
	if (c == 'f')
		return (FUNCT_SUCCESS);
	if (c == 'r')
		return (FUNCT_SUCCESS);
	if (c == '$')
		return (FUNCT_SUCCESS);
	if (c == '~')
		return (FUNCT_SUCCESS);
	return (FUNCT_FAILURE);
}

void			vsh_echo_escapedchars(char *arg)
{
	int index;
	int	is_escaped;
	int	is_special;
	int	len;

	index = 0;
	while (arg[index] != '\0')
	{
		is_special = vsh_echo_isspecial(arg[index]);
		is_escaped = vsh_ft_isescaped(arg, index);
		if (is_escaped && is_special)
		{
			len = ft_strlen(&(arg[index])) + 1;
			vsh_echo_replacespecial(&(arg[index]));
			ft_memmove(&(arg[index - 1]), &(arg[index]), len);
		}
		index++;
	}
}

void			vsh_echo(char **command_row)
{
	t_echo_flags	*flags;
	int				index;

	index = 1;
	flags = vsh_echo_getflags(command_row[1]);
	if (flags->flag_found)
		index++;
	while (command_row[index] != NULL)
	{
		if (!flags->cap_e_flag)
			vsh_echo_escapedchars(command_row[index]);
		ft_putstr(command_row[index]);
		if (command_row[index + 1])
			ft_putchar(' ');
		index++;
	}
	if (!flags->n_flag)
		ft_putchar('\n');
	free(flags);
}
 */