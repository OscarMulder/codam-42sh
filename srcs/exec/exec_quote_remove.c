/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   exec_quote_remove.c                                :+:    :+:            */
/*                                                     +:+                    */
/*   By: mavan-he <mavan-he@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/07/13 11:20:18 by mavan-he       #+#    #+#                */
/*   Updated: 2019/08/05 13:21:34 by mavan-he      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"

static void	remove_backslash(char *str, int *i, int *i_new)
{
	(*i)++;
	str[*i_new] = str[*i];
	(*i)++;
	(*i_new)++;
}

/*
**	In a expandable heredoc we only want to escape '\', '$', and
**	handle the special line continuation (escaped '\n').
*/

static void	remove_heredoc_backslash(char *str, int *i, int *i_new)
{
	(*i)++;
	if (str[*i] == '\\' || str[*i] == '$')
	{
		str[*i_new] = str[*i];
		(*i_new)++;
		(*i)++;
	}
	else if (str[*i] == '\n')
		(*i)++;
	else
		(*i)--;
}

static void	remove_double_quote(char *str, int *i, int *i_new)
{
	(*i)++;
	while (str[*i] != '"')
	{
		if (str[*i] == '\\' && ft_strchr("\"\\$", str[(*i) + 1]))
			remove_backslash(str, i, i_new);
		else
		{
			str[*i_new] = str[*i];
			(*i_new)++;
			(*i)++;
		}
	}
	(*i)++;
}

static void	remove_single_quote(char *str, int *i, int *i_new)
{
	(*i)++;
	while (str[*i] != '\'')
	{
		str[*i_new] = str[*i];
		(*i_new)++;
		(*i)++;
	}
	(*i)++;
}

void	tools_remove_quotes_etc(char *str, bool is_heredoc)
{
	int		i;
	int		i_new;

	i = 0;
	i_new = 0;
	while (str[i] != '\0')
	{
		if (str[i] == '\\' && is_heredoc == false)
			remove_backslash(str, &i, &i_new);
		else if (str[i] == '\\' && is_heredoc == true)
			remove_heredoc_backslash(str, &i, &i_new);
		else if (str[i] == '\'' && is_heredoc == false)
			remove_single_quote(str, &i, &i_new);
		else if (str[i] == '"' && is_heredoc == false)
			remove_double_quote(str, &i, &i_new);
		else
		{
			str[i_new] = str[i];
			i++;
			i_new++;
		}
	}
	ft_bzero(&str[i_new], i - i_new);
}

void		exec_quote_remove(t_ast *node)
{
	char	*str;
	bool	is_heredoc;

	if (node->left != NULL)
		exec_quote_remove(node->left);
	if (node->right != NULL)
		exec_quote_remove(node->right);
	if ((node->type == WORD || node->type == ASSIGN)
	&& (node->flags & T_FLAG_HEREDOC_NOEXP) == false)
	{
		is_heredoc = false;
		if (node->flags & T_FLAG_ISHEREDOC)
			is_heredoc = true;
		str = node->value;
		if (node->type == ASSIGN)
		{
			while (str[0] != '=')
				str++;
			str++;
		}
		tools_remove_quotes_etc(str, is_heredoc);
	}
}
