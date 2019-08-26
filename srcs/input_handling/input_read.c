/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   input_read.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: omulder <omulder@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/04/17 14:03:16 by jbrinksm       #+#    #+#                */
/*   Updated: 2019/08/26 15:32:53 by rkuijper      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"
#include <unistd.h>

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
	int start;
	int largest;

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

t_vshdata	*init_vshdata(t_vshdata *data)
{
	t_vshdata	*new;

	new = (t_vshdata*)ft_memalloc(sizeof(t_vshdata));
	if (new == NULL)
		return (NULL);
	new->c = 0;
	new->index = 0;
	new->len_cur = 0;
	new->len_max = 64;
	new->coords = (t_point){ 1 + data->prompt_len, 1 };
	new->hist_index = find_start(data->history);
	new->hist_start = new->hist_index - 1;
	new->hist_first = true;
	new->history = data->history;
	new->cur_ws_col = -1;
	return (new);
}

/*
**	Is called after reading '\e'. The complete sequence is dumped into a buf.
**	If we support the escape sequence, it is handled by any of the functions
**	below, otherwise it is ignored.
*/

int			input_read_ansi(t_vshdata *data)
{
	char	termcapbuf[TERMCAPBUFFSIZE];

	ft_bzero(termcapbuf, TERMCAPBUFFSIZE);
	if (data->c == '\e')
	{
		termcapbuf[0] = '\e';
		if (read(STDIN_FILENO, &termcapbuf[1], TERMCAPBUFFSIZE - 1) == -1)
			return (FUNCT_ERROR);
		if (ft_strequ(termcapbuf, TC_LEFT_ARROW) == true)
			curs_move_left(data, data);
		else if (ft_strequ(termcapbuf, TC_RIGHT_ARROW) == true)
			curs_move_right(data, data);
		else if (ft_strequ(termcapbuf, TC_HOME) == true)
			curs_go_home(data, data);
		else if (ft_strequ(termcapbuf, TC_END) == true)
			curs_go_end(data, data);
		else if (ft_strequ(termcapbuf, TC_DELETE) == true)
			input_handle_delete(data, data);
		else if (ft_strequ(termcapbuf, TC_UP_ARROW) == true)
			history_change_line(data, data, ARROW_UP);
		else if (ft_strequ(termcapbuf, TC_DOWN_ARROW) == true)
			history_change_line(data, data, ARROW_DOWN);
		else if (ft_strequ(termcapbuf, TC_CTRL_RIGHT_ARROW) == true)
			curs_move_next_word(data, data);
		else if (ft_strequ(termcapbuf, TC_CTRL_LEFT_ARROW) == true)
			curs_move_prev_word(data, data);
		else if (ft_strequ(termcapbuf, TC_CTRL_UP_ARROW) == true)
			curs_move_up(data, data);
		else if (ft_strequ(termcapbuf, TC_CTRL_DOWN_ARROW) == true)
			curs_move_down(data, data);
		else
		{
			ft_eprintf(">%s< TERMCAP NOT FOUND\n", &termcapbuf[1]); // DEBUG PRINT
			return (FUNCT_FAILURE);
		}
		return (FUNCT_SUCCESS);
	}
	return (FUNCT_FAILURE);
}

/*
**	Handles non-ansi single-byte special chars.
*/

int			input_read_special(t_vshdata *data)
{
	if (data->c == INPUT_BACKSPACE)
		input_handle_backspace(data, data);
	else if (data->c == INPUT_CTRL_D)
		input_parse_ctrl_d(data, data);
	else if (data->c == INPUT_CTRL_K)
		input_parse_ctrl_k(data, data);
	else if (data->c == INPUT_CTRL_U)
		input_parse_ctrl_u(data, data);
	else if (data->c == INPUT_CTRL_Y)
		input_parse_ctrl_y(data, data);
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

#include <sys/ioctl.h>
#include <term.h>

static int	input_resize_window_check(t_vshdata *data, t_vshdata *data)
{
	struct winsize	new;
	int				newlines;
	char			*tc_clear_lines_str;
	unsigned		saved_index;
	int				extra;

	ioctl(STDIN_FILENO, TIOCGWINSZ, &new);
	if (data->cur_ws_col == -1)
		data->cur_ws_col = new.ws_col;
	else if (data->cur_ws_col != new.ws_col)
	{
		saved_index = data->index; //save index
		newlines = data->coords.y - 1;
		extra = 0;
		if (data->cur_ws_col % new.ws_col > 0)
			extra = 1;
		newlines = newlines * ((data->cur_ws_col / new.ws_col) + extra);
		if (data->coords.x - 1 > 0)
			ft_printf("\e[%iD", data->coords.x - 1);
		ft_eprintf("NEWLINES: %i\n", newlines);
		if (newlines > 0)
			ft_printf("\e[%iA", newlines);
		tc_clear_lines_str = tgetstr("cd", NULL);
		if (tc_clear_lines_str == NULL)
		{
			ft_eprintf("ERROR\n"); // DEBUG PRINT
			return (FUNCT_ERROR); // do fatal shit
		}
		tputs(tc_clear_lines_str, 1, &ft_tputchar);
		shell_display_prompt(data, data->cur_prompt_type);
		sleep(1);
		data->index = data->len_cur;
		data->coords.x = 1 + (data->prompt_len + 1) % data->cur_ws_col; // + data->prompt_len;
		data->coords.y = 1 + (data->prompt_len + 1) / data->cur_ws_col;
		ft_eprintf("x: %i y: %i len: %i\n", data->coords.x, data->coords.y, data->prompt_len);
		data->cur_ws_col = new.ws_col;
		input_print_str(data, data->line);
		data->index = data->len_cur;
		curs_go_home(data, data);
		curs_move_n_right(data, data, saved_index);
	}
	return (FUNCT_SUCCESS);
}

int			input_read(t_vshdata *data /*will need ws.ws_col backup and cursor backup x and y*/)
{
	t_vshdata *data;

	data = init_vshdata(data);
	if (data == NULL)
		return (FUNCT_ERROR);
	data->line = ft_strnew(data->len_max);
	if (data->line == NULL)
		return (ft_free_return(data, FUNCT_ERROR));
	while (true)
	{
		input_resize_window_check(data, data);
		if (read(STDIN_FILENO, &data->c, 1) == -1)
			return (ft_free_return(data, FUNCT_ERROR));
		if (input_parse_ctrl_c(data, data) == FUNCT_SUCCESS)
			return (ft_free_return(data, NEW_PROMPT));
		else if (input_read_ansi(data, data) == FUNCT_FAILURE)
		{
			if (input_read_special(data, data) == FUNCT_FAILURE)
			{
				if (input_parse_char(data, data) == FUNCT_ERROR)
					return (ft_free_return(data, FUNCT_ERROR));
				if (data->c == '\n')
				{
					curs_go_end(data, data);
					break ;
				}
			}
		}
		data->c = '\0';
	}
	return (ft_free_return(data, FUNCT_SUCCESS));
}
