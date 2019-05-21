/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   lexer_evaluator.c                                  :+:    :+:            */
/*                                                     +:+                    */
/*   By: mavan-he <mavan-he@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/05/18 19:03:52 by mavan-he       #+#    #+#                */
/*   Updated: 2019/05/21 19:28:17 by jbrinksm      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"

/*
**	The evaluator removes '\', '\'' and '"' from token WORD and ASSIGN.
*/

static void	trim_forwardslash(char *str, int *i_new, int *i)
{
	if (str[*i] == '\\' && str[*i + 1] == '\\')
	{
		str[*i_new] = str[*i];
		(*i_new)++;
		(*i) += 2;
	}
	else
		(*i)++;
}

static void	trim_double_qoutes(char *str, int *i_new, int *i)
{
	(*i)++;
	while (str[*i] != '"')
	{
		if (str[*i] == '\\' && str[*i + 1] == '"')
		{
			(*i)++;
			str[*i_new] = str[*i];
			(*i_new)++;
			(*i)++;
		}
		else if (str[*i] == '\\')
			trim_forwardslash(str, i_new, i);
		else
		{
			str[*i_new] = str[*i];
			(*i_new)++;
			(*i)++;
		}
	}
	(*i)++;
}

static void	trim_single_qoutes(char *str, int *i_new, int *i)
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

static void	trim_word(char *str)
{
	int		i;
	int		i_new;

	i = 0;
	i_new = 0;
	while (str[i] != '\0')
	{
		if (str[i] == '\'')
			trim_single_qoutes(str, &i_new, &i);
		else if (str[i] == '"')
			trim_double_qoutes(str, &i_new, &i);
		else if (str[i] == '\\')
			trim_forwardslash(str, &i_new, &i);
		else
		{
			str[i_new] = str[i];
			i_new++;
			i++;
		}
	}
	ft_bzero(&str[i_new], i - i_new);
}

void		evaluator(t_tokenlst *lst)
{
	while (lst)
	{
		if (lst->type == WORD || lst->type == ASSIGN)
			trim_word((char*)lst->value.str);
		lst = lst->next;
	}
}
