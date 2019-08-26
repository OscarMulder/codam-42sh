/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   history_change_line.c                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: omulder <omulder@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/06/02 14:28:54 by mavan-he       #+#    #+#                */
/*   Updated: 2019/08/23 15:09:50 by rkuijper      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"
#include "libft.h"
#include <term.h>

/*
**	Cursor (and index) will be moved to the start of the string. The screen will
**	be cleared from that line and onwards. A new prompt will be displayed.
*/

static void	history_clear_line(t_inputdata *data, t_vshdata *vshdata)
{
	char	*tc_clear_lines_str;

	curs_go_home(data, vshdata);
	tc_clear_lines_str = tgetstr("cd", NULL);
	if (tc_clear_lines_str == NULL)
	{
		ft_eprintf("ERROR\n");
		return ; // do fatal shit
	}
	tputs(tc_clear_lines_str, 1, &ft_tputchar);
}

static int	malloc_and_copy(t_inputdata *data, char **line, char *str)
{
	unsigned	len;

	len = ft_strlen(str);
	if (len < data->len_max)
	{
		ft_bzero(*line, data->len_max);
	}
	else
	{
		while (len >= data->len_max)
			data->len_max *= 2;
		ft_strdel(&(*line));
		*line = ft_strnew(data->len_max);
		if (*line == NULL)
			return (FUNCT_ERROR);
	}
	ft_strcpy(*line, str);
	return (FUNCT_SUCCESS);
}

static int	set_line(t_inputdata *data, char **line)
{
	if (malloc_and_copy(data, line, data->history[data->hist_index]->str)
	== FUNCT_ERROR)
		return (FUNCT_ERROR);
	return (FUNCT_SUCCESS);
}

int			history_change_line(t_inputdata *data, t_vshdata *vshdata,
		char arrow)
{
	history_clear_line(data, vshdata);
	if (arrow == ARROW_UP)
	{
		if (history_index_change_up(data))
			set_line(data, &vshdata->line);
		else
			ft_printf("\a");
	}
	else if (arrow == ARROW_DOWN)
	{
		if (history_index_change_down(data))
			set_line(data, &vshdata->line);
		else
			ft_bzero(vshdata->line, data->len_max);
	}
	input_print_str(data, vshdata->line);
	data->index = data->len_cur = ft_strlen(vshdata->line);
	return (FUNCT_SUCCESS);
}
