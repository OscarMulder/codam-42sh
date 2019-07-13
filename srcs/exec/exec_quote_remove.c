/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   exec_quote_remove.c                                :+:    :+:            */
/*                                                     +:+                    */
/*   By: mavan-he <mavan-he@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/07/13 11:20:18 by mavan-he       #+#    #+#                */
/*   Updated: 2019/07/13 13:18:02 by mavan-he      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"

static void	remove_backslash(char *str)
{
	int i;
	int i_new;

	i = 0;
	i_new = 0;
	while (str[i] != '\0')
	{
		if (str[i] == '\\' && str[i + 1] == '\\')
		{
			str[i_new] = str[i];
			i_new++;
			i += 2;
		}
		else if (str[i] == '\\')
			i++;
		else
		{
			str[i_new] = str[i];
			i++;
			i_new++;
		}
	}
	ft_bzero(&str[i_new], i - i_new);
}

static void	remove_first_last(char *str)
{
	int len;

	len = ft_strlen(str);
	str[len - 1] = '\0';
	ft_memmove(str, &str[1], len);
}

static char	*return_node_str(t_ast *node)
{
	char *str;

	if (node->type == ASSIGN)
	{
		str = node->value;
		while (str[0] != '=')
			str++;
		str++;
		return (str);
	}
	else
		return (node->value);
}

void		exec_quote_remove(t_ast *node)
{
	char *str;

	if (node->child != NULL)
		exec_quote_remove(node->child);
	if (node->sibling != NULL)
		exec_quote_remove(node->sibling);
	if (node->type == WORD || node->type == ASSIGN)
	{
		str = return_node_str(node);
		if (str[0] == '"')
		{
			remove_first_last(str);
			remove_backslash(str);
		}
		else if (str[0] == '\'')
			remove_first_last(str);
		else
			remove_backslash(str);
		ft_putstr("test: ");
		ft_putstr(node->value);
	}
}
