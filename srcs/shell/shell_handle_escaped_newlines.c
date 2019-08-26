/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   shell_handle_escaped_newlines.c                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: jbrinksm <jbrinksm@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/07/31 14:18:35 by jbrinksm       #+#    #+#                */
/*   Updated: 2019/08/07 11:18:46 by jbrinksm      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"

static bool	remove_last_escaped_newline(char *line)
{
	int i;

	i = ft_strlen(line);
	if (i == 0)
		return (false);
	i--;
	if (line[i] == '\n' && tools_is_char_escaped(line, i) == true)
	{
		line[i] = '\0';
		line[i - 1] = '\0';
		return (true);
	}
	return (false);
}

int			shell_handle_escaped_newlines(t_vshdata *data)
{
	int		ret;
	char	*line_tmp;

	ret = remove_last_escaped_newline(data->line->line);
	if (ret == false)
		return (FUNCT_FAILURE);
	while (ret != false)
	{
		ft_putstr("\nlinecont> ");
		line_tmp = data->line->line;
		data->line->line = NULL;
		input_read(data);
		data->line->line = ft_strjoinfree_all(line_tmp, data->line->line);
		if (data->line->line == NULL)
			return (FUNCT_ERROR); // print error func PLZ
		ret = remove_last_escaped_newline(data->line->line);
	}
	return (FUNCT_SUCCESS);
}
