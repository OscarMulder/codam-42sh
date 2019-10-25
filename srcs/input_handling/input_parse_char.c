/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   input_parse_char.c                                 :+:    :+:            */
/*                                                     +:+                    */
/*   By: omulder <omulder@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/05/16 13:33:54 by rkuijper       #+#    #+#                */
/*   Updated: 2019/10/25 13:47:20 by omulder       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"
#include "term.h"

/*
**  This function makes sure that when you are inserting a char
**  inbetween other chars on the line, that the chars on the right
**  side are moved over by 1, which creates a gap for the char
**  that needs to be inserted.
*/

static void	create_char_gap(char *line, int len, int gap_index)
{
	int i;

	i = len - 1;
	while (i >= gap_index)
	{
		line[i + 1] = line[i];
		i--;
	}
}

/*
**  Inserts a char `c` at `index` on `*line`.
**  When there is not enough space on `*line`, the amount
**  of memory allocated for `*line` is doubled first.
*/

static int	add_char_at(t_vshdata *data, int i, char c, char **line)
{
	char *tmp;

	if (data->line->len_cur < data->line->len_max)
	{
		create_char_gap(*line, data->line->len_cur, i);
		(*line)[i] = c;
	}
	else
	{
		while (data->line->len_cur >= data->line->len_max)
			data->line->len_max *= 2;
		tmp = ft_strnew(data->line->len_max);
		if (tmp == NULL)
			return (FUNCT_ERROR);
		ft_strcpy(tmp, *line);
		ft_strdel(line);
		create_char_gap(tmp, data->line->len_cur, i);
		tmp[i] = c;
		*line = tmp;
	}
	data->line->len_cur++;
	return (FUNCT_SUCCESS);
}

/*
**  Makes sure that when a `\n` char is caught, it is put
**  at the end of `*line`. When there is not enough space
**  on line, the amount of memory for `*line` is increased
**  by 1 byte first.
*/

static int	add_newline(t_vshdata *data, char **line)
{
	char *tmp;

	if (data->line->len_cur < data->line->len_max)
		(*line)[data->line->len_cur] = '\n';
	else
	{
		data->line->len_max += 1;
		tmp = ft_strnew(data->line->len_max);
		if (tmp == NULL)
			return (FUNCT_ERROR);
		ft_strcpy(tmp, *line);
		ft_strdel(line);
		tmp[data->line->len_cur] = '\n';
		*line = tmp;
	}
	return (FUNCT_SUCCESS);
}

/*
**	The following function gets called after a normal character
**	has been read. This is to make sure the whole buffer gets read
**	before we start to accept new input (whole buffer as in a paste
**	for example).
**	It starts off by setting an fd_set to STDIN so we can read the
**	current status of our input. The timeval struct is basically
**	arbitrary in this case, since we don't want any timeout
**	before our 'status check'; input should already be available (if any).
**	The select function returns a status of the given file descriptors
**	which, in this case, is just the STDIN (the NULL, NULL
**	parameters can be used to also include an out- and error fd,
**	but we just want input). If this status is not equal to 0 (1
**	in most cases) we know that the STDIN buffer still contains some data
**	so we read out a character and put it in the line buffer.
**	This function is called recursively until the input buffer is once again
**	empty, after which it'll return the amount of characters it read from
**	from the buffer (n - 1).
*/

int			input_empty_buffer(t_vshdata *data, int n)
{
	fd_set			readfds;
	struct timeval	timeout;
	int				bytes_read;
	char			buf[INPUT_BUF_READ_SIZE + 1];

	FD_ZERO(&readfds);
	timeout.tv_sec = 0;
	timeout.tv_usec = 0;
	FD_SET(STDIN_FILENO, &readfds);
	if (select(1, &readfds, NULL, NULL, &timeout) != 0)
	{
		bytes_read = read(STDIN_FILENO, buf, INPUT_BUF_READ_SIZE);
		buf[bytes_read] = '\0';
		input_add_chunk(data, buf, bytes_read);
		return (input_empty_buffer(data, n + bytes_read));
	}
	return (n - 1);
}

static int			input_parse_char_og(t_vshdata *data)
{
	int old_index;

	if (ft_isprint(data->input->c))
	{
		if (add_char_at(data, data->line->index, data->input->c,
			&data->line->line) == FUNCT_ERROR)
			return (FUNCT_ERROR);
		old_index = data->line->index;
		input_print_str(data, data->line->line + data->line->index);
		data->line->index = data->line->len_cur;
		curs_move_n_left(data, data->line->index - old_index - 1);
	}
	else if (data->input->c == '\n')
	{
		if (add_newline(data, &data->line->line) == FUNCT_ERROR)
			return (FUNCT_ERROR);
	}
	return (FUNCT_SUCCESS);
}

void	ctrlr_clear_line(t_vshdata *data)
{
	// curs_move_n_left(data, data->line->index);
	// data->line->index = ft_strlen(HIST_SRCH_FIRST "" HIST_SRCH_LAST);
	// if (data->input->searchhistory.result_str[0] != '\0')
	// 	data->line->index += data->input->searchhistory.result_i + 1;
	// curs_move_n_left(data, data->line->index);
	// tputs(data->termcaps->tc_clear_lines_str, 1, &ft_tputchar);
	ft_eprintf("CLEAR LINE total: %d\n", data->input->searchhistory.total_len);
	data->line->index = data->input->searchhistory.total_len;
	curs_move_n_left(data, data->input->searchhistory.total_len);
	tputs(data->termcaps->tc_clear_lines_str, 1, &ft_tputchar);
}

int			input_parse_char(t_vshdata *data)
{
	if (data->input->searchhistory.active)
	{
		ft_eprintf("res_i: %d index: %d\n", data->input->searchhistory.result_i, data->line->index);
		ctrlr_clear_line(data);
		data->line->index = data->line->len_cur;
		input_print_str(data, HIST_SRCH_FIRST);
		input_print_str(data, data->line->line);
		if (input_parse_char_og(data) == FUNCT_ERROR)
			return (FUNCT_ERROR);
		input_print_str(data, HIST_SRCH_LAST);
		if (history_ctrl_r(data, false) == FUNCT_ERROR)
			return (FUNCT_ERROR);
		input_print_str(data, data->input->searchhistory.result_str);
		data->line->index = ft_strlen(data->input->searchhistory.result_str);
		curs_move_n_left(data, data->line->index - data->input->searchhistory.result_i);
		data->input->searchhistory.total_len = ft_strlen(HIST_SRCH_FIRST "" HIST_SRCH_LAST) + data->line->len_cur + data->input->searchhistory.result_i;
		data->line->index = data->line->len_cur;
		ft_eprintf("PARSE CHAR total: %d\n", data->input->searchhistory.total_len);
	}
	else
		input_parse_char_og(data);
	return (FUNCT_SUCCESS);
}