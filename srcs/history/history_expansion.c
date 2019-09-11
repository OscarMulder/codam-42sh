/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   history_expansion.c                                :+:    :+:            */
/*                                                     +:+                    */
/*   By: mavan-he <mavan-he@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/09/10 12:37:29 by mavan-he       #+#    #+#                */
/*   Updated: 2019/09/11 17:43:32 by mavan-he      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

/*
**	!      		Start a history substitution, except when followed by:
**				a space, tab, the end of the line, '=' or '('.
**	!n     		Refer to command line n. 
**	!-n    		Refer to the command n lines back. 
**	!!     		Refer to the previous command. This is a synonym for '!-1'. 
**	!string		Refer to the most recent command starting with string. 
*/

#include "vsh.h"

int		history_expand(t_datahistory *history, char **line, size_t *i)
{
	char *history_line;
	if (ft_strchr("\n\t =(", (*line)[*i + 1]) != NULL
		|| (*line)[*i + 1] == '\0')
		return (FUNCT_SUCCESS);
	history_line = history_get_line(history, *line, *i);
	if (history_line == NULL)
		return (FUNCT_FAILURE); // print error
	// replace !x with history_line and set i to end of expan
	return (FUNCT_SUCCESS);
}

int		history_expansion(t_vshdata *data)
{
	size_t		i;
	int		state;

	state = 0;
	i = 0;
	while (data->line->line[i])
	{
		if (data->line->line[i])
		{
			if (data->line->line[i] == '\\')
				i++;
			else if (data->line->line[i] == '\'')
				state ^= T_STATE_SQUOTE;
			else if (data->line->line[i] == '!' &&
				(state & T_STATE_SQUOTE) == false)
			{
				if (history_expand(data->history, &data->line->line, &i)
					!= FUNCT_SUCCESS)
					return (FUNCT_ERROR);
			}
			i++;
		}
	}
	return (FUNCT_SUCCESS);
}
