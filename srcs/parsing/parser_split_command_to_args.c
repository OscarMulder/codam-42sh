/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parser_split_command_to_args.c                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: jbrinksm <marvin@codam.nl>                   +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/05/02 20:48:21 by jbrinksm       #+#    #+#                */
/*   Updated: 2019/05/05 12:25:02 by jbrinksm      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"

/*
**	Useful function to check 
*/

int		is_uninhibited_blank(char *str, int i, char quote)
{
	if ((str[i] == ' ' || str[i] == '\t') && quote == '\0')
	{
		if (is_char_escaped(str, i) == FUNCT_SUCCESS)
			return (FUNCT_FAILURE);
		return (FUNCT_SUCCESS);
	}
	return (FUNCT_FAILURE);
}

int		parser_strlen_arg(char *cmd)
{
	int		i;
	char	quote;

	i = 0;
	quote = '\0';
	update_quote_status(cmd, i, &quote);
	while (cmd[i] != '\0' && is_uninhibited_blank(cmd, i, quote) != true)
	{
		i++;
		update_quote_status(cmd, i, &quote);
	}
	return (i);
}

t_list	*parser_split_command_to_args(char *cmd)
{
	t_list		*arg_lst;
	char		*arg;
	int			i;
	int			len;

	arg_lst = NULL;
	i = 0;
	while (cmd[i] != '\0')
	{
		while (ft_isblank(cmd[i]) == true)
			i++;
		len = parser_strlen_arg(&cmd[i]);
		if (len > 0)
		{
			arg = ft_strndup(&cmd[i], len);
			parser_add_str_to_lst(arg, &arg_lst);
			ft_strdel(&arg);
			i += len;
		}
		else
			i++;
	}
	return (arg_lst);
}
