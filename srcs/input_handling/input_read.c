/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   input_read.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: omulder <omulder@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/04/17 14:03:16 by jbrinksm       #+#    #+#                */
/*   Updated: 2019/08/23 18:06:26 by jbrinksm      ########   odam.nl         */
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

t_inputdata	*init_inputdata(t_vshdata *vshdata)
{
	t_inputdata	*new;

	new = (t_inputdata*)ft_memalloc(sizeof(t_inputdata));
	if (new == NULL)
		return (NULL);
	new->c = 0;
	new->index = 0;
	new->len_cur = 0;
	new->len_max = 64;
	new->coords = (t_point){ 1 + vshdata->prompt_len, 1 };
	new->hist_index = find_start(vshdata->history);
	new->hist_start = new->hist_index - 1;
	new->hist_first = true;
	new->history = vshdata->history;
	new->cur_ws_col = -1;
	return (new);
}

/*
**	Is called after reading '\e'. The complete sequence is dumped into a buf.
**	If we support the escape sequence, it is handled by any of the functions
**	below, otherwise it is ignored.
*/

int			input_read_ansi(t_inputdata *data, t_vshdata *vshdata)
{
	char	termcapbuf[TERMCAPBUFFSIZE];

	ft_bzero(termcapbuf, TERMCAPBUFFSIZE);
	if (data->c == '\e')
	{
		termcapbuf[0] = '\e';
		if (read(STDIN_FILENO, &termcapbuf[1], TERMCAPBUFFSIZE - 1) == -1)
			return (FUNCT_ERROR);
		if (ft_strequ(termcapbuf, TC_LEFT_ARROW) == true)
			curs_move_left(data);
		else if (ft_strequ(termcapbuf, TC_RIGHT_ARROW) == true)
			curs_move_right(data, vshdata);
		else if (ft_strequ(termcapbuf, TC_HOME) == true)
			curs_go_home(data);
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
			return (FUNCT_FAILURE);
		}
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

static int	input_resize_window_check(t_vshdata *vshdata, t_inputdata *data)
{
	struct winsize	new;
	t_point			new_coords;
	int				newlines;
	char			*tc_clear_lines_str;
	unsigned		saved_index;
	int				extra;

	(void)vshdata;
	ioctl(STDIN_FILENO, TIOCGWINSZ, &new);
	if (data->cur_ws_col == -1)
		data->cur_ws_col = new.ws_col;
	else if (data->cur_ws_col != new.ws_col)
	{
		saved_index = data->index; //save index
		ft_eprintf("old x: %i - y: %i - col: %i\n", data->coords.x, data->coords.y, data->cur_ws_col);
		new_coords.x = 1 + ((data->coords.y - 1) * data->cur_ws_col + (data->coords.x - 1)) % new.ws_col;
		new_coords.y = 1 + ((data->coords.y - 1) * data->cur_ws_col + (data->coords.x - 1)) / new.ws_col;
		newlines = data->coords.y - 1;
		extra = 0;
		if (data->cur_ws_col % new.ws_col > 0)
			extra = 1;
		newlines = newlines * ((data->cur_ws_col / new.ws_col) + extra);
		if (data->coords.x - 1 > 0)
			ft_printf("\e[%iD", data->coords.x - 1);
		sleep (1);
		ft_eprintf("NEWLINES: %i\n", newlines);
		if (newlines > 0)
			ft_printf("\e[%iA", newlines);
		sleep (1);
		tc_clear_lines_str = tgoto(tgetstr("cd", NULL), 0, 1);
		if (tc_clear_lines_str == NULL)
		{
			ft_eprintf("ERROR\n"); // DEBUG PRINT
			return (FUNCT_ERROR); // do fatal shit
		}
		tputs(tc_clear_lines_str, 1, &ft_tputchar);
		sleep(1);
		shell_display_prompt(vshdata, vshdata->cur_prompt_type);
		sleep(1);
		data->index = ft_strlen(vshdata->line);
		data->coords.x = 1 + vshdata->prompt_len;
		data->coords.y = 1;
		data->cur_ws_col = new.ws_col;
		input_print_str(data, vshdata->line);
		data->index = data->len_cur;
		sleep(1);
		curs_go_home(data);
		sleep(1);
		curs_move_n_right(data, vshdata, saved_index);
		ft_eprintf("new x: %i - y: %i - col: %i - nl: %i\n", new_coords.x, new_coords.y, new.ws_col, newlines);
	}
	return (FUNCT_SUCCESS);
}

int			input_read(t_vshdata *vshdata /*will need ws.ws_col backup and cursor backup x and y*/)
{
	t_inputdata *data;
	int			ret;

	data = init_inputdata(vshdata);
	if (data == NULL)
		return (FUNCT_ERROR);
	vshdata->line = ft_strnew(data->len_max);
	if (vshdata->line == NULL)
		return (ft_free_return(data, FUNCT_ERROR));
	while (true)
	{
		input_resize_window_check(vshdata, data);
		ret = read(STDIN_FILENO, &data->c, 1);
		if (ret == -1)
			return (ft_free_return(data, FUNCT_ERROR));
		else if (ret == 0)
			continue ;
		if (input_parse_ctrl_c(data, vshdata) == FUNCT_SUCCESS)
			return (ft_free_return(data, NEW_PROMPT));
		else if (input_read_ansi(data, vshdata) == FUNCT_FAILURE)
		{
			if (input_read_special(data, vshdata) == FUNCT_FAILURE)
			{
				if (input_parse_char(data, vshdata) == FUNCT_ERROR)
					return (ft_free_return(data, FUNCT_ERROR));
				if (data->c == '\n')
				{
					curs_go_end(data, vshdata);
					break ;
				}
			}
		}
		ft_eprintf("AFT: index: %i/%i\n", data->index, data->len_cur); // DEBUG PRINT
	}
	return (ft_free_return(data, FUNCT_SUCCESS));
}
