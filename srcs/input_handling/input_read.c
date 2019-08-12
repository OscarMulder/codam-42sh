/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   input_read.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: omulder <omulder@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/04/17 14:03:16 by jbrinksm       #+#    #+#                */
/*   Updated: 2019/08/12 14:52:45 by rkuijper      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"
#include <unistd.h>
#include <sys/ioctl.h> // REMOVE
#include <term.h> //remove

void		input_clear_char_at(char **line, unsigned index)
{
	unsigned i;

	i = index;
	while ((*line)[i])
	{
		(*line)[i] = (*line)[i + 1];
		i++;
	}
}

static int	find_start(t_history **history)
{
	int i;
	int largest;
	int start;

	i = 0;
	start = 0;
	largest = -1;
	while (i < HISTORY_MAX && history[i]->str != NULL)
	{
		if (history[i]->number > largest)
		{
			start = i;
			largest = history[i]->number;
		}
		i++;
	}
	return (start + 1);
}

t_inputdata	*init_inputdata(t_vshdata *vshdata)
{
	t_inputdata	*new;

	new = (t_inputdata*)ft_memalloc(sizeof(t_inputdata));
	if (new == NULL)
		return (NULL);
	new->c = 0;
	new->index = 0;
	new->hist_index = find_start(vshdata->history);
	new->hist_start = new->hist_index - 1;
	new->hist_first = true;
	new->history = vshdata->history;
	new->len_cur = 0;
	new->len_max = 64;
	return (new);
}

/*
**	Sends termcap sequence which requests your cursors current location
**	and then reads the response from STDIN.
*/

static char	*get_cursor_pos(void)
{
	char	*buf;
	int		ret;

	ft_putstr(TC_GETCURSORPOS);
	buf = ft_strnew(100);
	ret = read(STDIN_FILENO, buf, 100);
	if (ret == -1)
	{
		ft_strdel(&buf);
		return (NULL);
	}
	return (buf);
}

/*
**	Beware: crappy function
**
**	Parses the response to get the current x pos or column (starts at 1).
**	VERY DANGEROUS PLEASE IMPROVE IT.
*/

int		get_cursor_linepos(void)
{
	char	*response;
	int		i;

	response = get_cursor_pos();
	if (response == NULL)
		return (-1);
	i = 0;
	while (response[i] != '\0' && response[i] != ';')
		i++;
	if (response[i] == '\0' || response[i + 1] == '\0'
		|| ft_isdigit(response[i + 1]) == false)
	{
		ft_strdel(&response);
		return (-1);
	}
	return ((short)ft_atoi(&response[i + 1]) /*needs unsigned atoi and short  ret */);
}

/*
**	Beware: crappy function
**
**	Parses the response to get the current y pos or row (starts at 1).
**	VERY DANGEROUS PLEASE IMPROVE IT.
*/

int		get_cursor_rowpos(void)
{
	char	*response;
	int		i;

	response = get_cursor_pos();
	if (response == NULL)
		return (-1);
	i = 0;
	while (response[i] != '\0' && response[i] != '[')
		i++;
	if (response[i] == '\0' || response[i + 1] == '\0'
		|| ft_isdigit(response[i + 1]) == false)
	{
		ft_strdel(&response);
		return (-1);
	}
	return ((short)ft_atoi(&response[i + 1]) /*needs unsigned atoi and short  ret */);
}

/*
**	Is called after reading '\e'. The complete sequence is dumped into a buf.
**	If we support the escape sequence, it is handled by any of the functions
**	below, otherwise it is ignored.
*/

int			input_read_ansi(t_inputdata *data, t_vshdata *vshdata)
{
	char	*termcapbuf;

	if (data->c == '\e')
	{
		termcapbuf = ft_strnew(12);
		if (termcapbuf == NULL)
		{
			// do fatal shit
			return (FUNCT_ERROR);
		}
		*termcapbuf = '\e';
		if (read(STDIN_FILENO, &termcapbuf[1], TERMCAPBUFFSIZE - 1) == -1)
		{
			ft_strdel(&termcapbuf);
			return (FUNCT_ERROR);
		}
		if (ft_strequ(termcapbuf, TC_LEFT_ARROW) == true)
			curs_move_left(data);
		else if (ft_strequ(termcapbuf, TC_RIGHT_ARROW) == true)
			curs_move_right(data);
		else if (ft_strequ(termcapbuf, TC_HOME) == true)
			curs_go_home(data, vshdata);
		else if (ft_strequ(termcapbuf, TC_END) == true)
			curs_go_end(data, vshdata);
		else if (ft_strequ(termcapbuf, TC_DELETE) == true)
			input_handle_delete(data, vshdata);
		else if (ft_strequ(termcapbuf, TC_UP_ARROW) == true)
			history_change_line(data, vshdata, ARROW_UP);
		else if (ft_strequ(termcapbuf, TC_DOWN_ARROW) == true)
			history_change_line(data, vshdata, ARROW_DOWN);
		else if (ft_strequ(termcapbuf, TC_CTRL_RIGHT_ARROW) == true)
			curs_move_next_word(data, vshdata);
		else if (ft_strequ(termcapbuf, TC_CTRL_LEFT_ARROW) == true)
			curs_move_prev_word(data, vshdata);
		else if (ft_strequ(termcapbuf, TC_CTRL_UP_ARROW) == true)
			curs_move_up(data, vshdata);
		else if (ft_strequ(termcapbuf, TC_CTRL_DOWN_ARROW) == true)
			curs_move_down(data, vshdata);
		else
		{
			ft_eprintf(">%s< TERMCAP NOT FOUND\n", &termcapbuf[1]); // DEBUG PRINT
			ft_strdel(&termcapbuf);
			return (FUNCT_FAILURE);
		}
		ft_strdel(&termcapbuf);
		return (FUNCT_SUCCESS);
	}
	return (FUNCT_FAILURE);
}

/*
**	Handles non-ansi single-byte special chars.
*/

int			input_read_special(t_inputdata *data, t_vshdata *vshdata)
{
	if (data->c == INPUT_BACKSPACE)
		input_handle_backspace(data, vshdata);
	else if (data->c == INPUT_CTRL_D)
		input_parse_ctrl_d(data, vshdata);
	else if (data->c == INPUT_CTRL_K)
		input_parse_ctrl_k(data, vshdata);
	else
		return (FUNCT_FAILURE);
	return (FUNCT_SUCCESS);
}

/*
**	Welcome Rob!
**	I have left you many comments to help you understand why and how I have
**	changed a lot (basically all) of the functions. Before you read it all,
**	I want to let you know a handy debugging tip which is the following:
**	1. run vsh as `./vsh 2>TAILME.log`
**	2. Open a second terminal window and run `tail -f TAILME.log`
**	3. Profit from all the debug messages that get displayed.
**	4. ???
**
**	The new input read will loop forever until a '\n' is caught, or any other
**	return is given.
**
**	In the loop:
**
**	First, the current screen size will be
**	compared with the saved size. If the size changed, oscars function will
**	make sure that everything is reprinted, and that the cursor and index
**	will be properly recovered. (NO FUNCTIONS AFTER THIS SHOULD HAVE TO DEAL
**	WITH CHANGES TO THE SCREEN SO THEY WILL BE ENTIRELY DEPENDANT ON PROPER
**	RESIZE HANDLING)
**
**	Then, one char is read, if this char is a `\e`, input_read_ansi is used
**	to read the totality of the escape sequence into a buffer. If the escape
**	sequence is supported, we run the corresponding function, otherwise it
**	should be ignored.
**
**	If the char has any other special meaning like INPUT_BACKSPACE or
**	INPUT_CTRL_D and is handled.
**
**	Any other char will be parsed by input_parse_char. If it happened to be
**	a '\n' the input reading is complete, and we break.
**
**
**	PS: As you can see I got rid of your char/input state system, because it
**	didn't need to be that complicated (:
**
**	PPS: Please use functions like `get_cursor_rowpos` (and maybe also ioctl),
**	as sparingly as possible because they can REALLY make the shell lag like a
**	bitch and also glitch out because it then can't handle the input speed.
**	Also, try to be as efficient as possible with using termcaps for
**	for example cursor movement. (But I think you already knew that.)
**
**	GL & HF
*/

int			input_read(t_vshdata *vshdata /*will need ws.ws_col backup and cursor backup x and y*/)
{
	t_inputdata *data;

	data = init_inputdata(vshdata);
	if (data == NULL)
		return (FUNCT_ERROR);
	vshdata->line = ft_strnew(data->len_max);
	if (vshdata->line == NULL)
		return (ft_free_return(data, FUNCT_ERROR));
	while (true)
	{
		// get and compare new ws.ws_col
		// ft_eprintf("BEF: index: >%i<\n", data->index);
		if (read(STDIN_FILENO, &data->c, 1) == -1)
			return (ft_free_return(data, FUNCT_ERROR));
		if (input_parse_ctrl_c(data, vshdata) == FUNCT_SUCCESS)
			return (ft_free_return(data, NEW_PROMPT));
		else if (input_read_ansi(data, vshdata) == FUNCT_FAILURE)
		{
			if (input_read_special(data, vshdata) == FUNCT_FAILURE)
			{
				if (input_parse_char(data, &vshdata->line) == FUNCT_ERROR)
					return (ft_free_return(data, FUNCT_ERROR));
				if (data->c == '\n')
					break ;
			}
		}
		ft_eprintf("AFT: index: %i/%i\n", data->index, data->len_cur); // DEBUG PRINT
	}
	return (ft_free_return(data, FUNCT_SUCCESS));
}
