/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   auto_start.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: mavan-he <mavan-he@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/08/12 14:09:10 by mavan-he       #+#    #+#                */
/*   Updated: 2019/08/12 21:04:21 by mavan-he      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"

char	*auto_get_match_str(char *line, int i)
{
	int		i_cursor;
	char	*match;

	i_cursor = i;
	i--;
	while (i >= 0)
	{
		if (tools_isidentifierchar(line[i]) == false)
			break;
		i--;
	}
	match = ft_strnew(i_cursor - i);
	if (match == NULL)
		return (NULL); // error msg
	ft_strncpy(match, &line[i + 1], (i_cursor - i) - 1);
	match[i_cursor - i] = '\0';
	return (match);
}

int		auto_start(t_vshdata *vshdata, int *i)
{
	int		state;
	char	*match;

	// vshdata line toevoegen;
	state = auto_find_state(vshdata->line, *i);
	match = NULL;
	if (state == STATE_CMD)
		match = auto_get_match_str(vshdata->line, *i); // putstr for bug fixing only
	else if (state == STATE_VAR)
		match = auto_get_match_str(vshdata->line, *i); // putstr for bug fixing only
	else if (state == STATE_FILE)
		;// find file
	if (match == NULL)
		return (FUNCT_ERROR); // error msg
	
	return (auto_find_matches(vshdata, match, i, state));
}
