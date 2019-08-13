/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   auto_start.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: mavan-he <mavan-he@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/08/12 14:09:10 by mavan-he       #+#    #+#                */
/*   Updated: 2019/08/13 17:38:18 by mavan-he      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"

void	auto_lstdel(void *str, size_t size)
{
	(void)size;
	ft_strdel((char**)&str);
}

char	*auto_get_file_str(char *line, int i)
{
	int		i_cursor;
	char	*match;

	i_cursor = i;
	i--;
	while (i >= 0)
	{
		if (tools_isidentifierchar(line[i]) == false &&
			line[i] != '/' && line[i] != '.')
			break;
		i--;
	}
	match = ft_strnew(i_cursor - i);
	if (match == NULL)
	{
		ft_eprintf("vsh: failed to allocate enough memory\n");
		return (NULL);
	}
	ft_strncpy(match, &line[i + 1], (i_cursor - i) - 1);
	match[i_cursor - i] = '\0';
	return (match);
}

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
	{
		ft_eprintf("vsh: failed to allocate enough memory\n");
		return (NULL);
	}
	ft_strncpy(match, &line[i + 1], (i_cursor - i) - 1);
	match[i_cursor - i] = '\0';
	return (match);
}

int		auto_start(t_vshdata *vshdata, int *i)
{
	int		state;
	char	*match;
	t_list	*matchlst;

	state = auto_find_state(vshdata->line, *i);
	ft_printf("\n<<<<< State = %d >>>>>>\n", state); // Debugging
	match = NULL;
	matchlst = NULL;
	if (state == STATE_CMD)
		match = auto_get_match_str(vshdata->line, *i);
	else if (state == STATE_VAR)
		match = auto_get_match_str(vshdata->line, *i);
	else if (state == STATE_FILE)
		match = auto_get_file_str(vshdata->line, *i);
	ft_printf("<<<<< Match = %s >>>>>>\n", match); // Debugging
	if (match == NULL ||
		auto_find_matches(vshdata, &match, &matchlst, state) == FUNCT_ERROR)
		state = FUNCT_ERROR;
	else
		state = auto_handle_matchlst(vshdata, i, match, &matchlst);
	ft_strdel(&match);
	ft_lstdel(&matchlst, &auto_lstdel);
	return (state);
}
