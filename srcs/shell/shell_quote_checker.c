/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   shell_quote_checker.c                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: jbrinksm <jbrinksm@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/05/31 07:47:19 by jbrinksm       #+#    #+#                */
/*   Updated: 2019/08/03 12:13:59 by rkuijper      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"
#include <unistd.h>

char	shell_quote_checker_find_quote(char *line)
{
	int		i;
	char	quote;
	char	c;

	i = 0;
	quote = '\0';
	while (line[i] != '\0')
	{
		if (quote != '\'' && tools_is_char_escaped(line, i) == true)
		{
			i++;
			continue ;
		}
		c = line[i];
		if (quote == '\0' && (c == '\'' || c == '"'))
			quote = c;
		else if (quote != '\0' && c == quote)
			quote = '\0';
		i++;
	}
	return (quote);
}

/*
**	Right now there is no way of exiting the program through regular signals
**	if you don't supply the proper END_HERE input. Will need to be fixed through
**	some termcaps shit.
*/

int		shell_close_unclosed_quotes(t_vshdata *vshdata)
{
	char	quote;
	char	*line_tmp;

	quote = shell_quote_checker_find_quote(vshdata->line);
	while (quote != '\0')
	{
		line_tmp = vshdata->line;
		vshdata->line = NULL;
		if (quote == '\'')
			ft_printf("\nquote> ");
		else if (quote == '"')
			ft_printf("\ndquote> ");
		if (input_read(vshdata) == FUNCT_ERROR)
		{
			ft_strdel(&line_tmp);
			return (FUNCT_ERROR);
		}
		vshdata->line = ft_strjoinfree_all(line_tmp, vshdata->line);
		if (vshdata->line == NULL)
		{
			ft_eprintf("vsh: failed to allocate enough memory\n");
			return (FUNCT_ERROR);
		}
		quote = shell_quote_checker_find_quote(vshdata->line);
	}
	return (FUNCT_SUCCESS);
}
