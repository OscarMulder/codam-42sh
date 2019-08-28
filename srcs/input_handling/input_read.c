/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   input_read.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: omulder <omulder@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/04/17 14:03:16 by jbrinksm       #+#    #+#                */
/*   Updated: 2019/08/28 20:58:00 by jbrinksm      ########   odam.nl         */
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

/*
**	Is called after reading '\e'. The complete sequence is dumped into a buf.
**	If we support the escape sequence, it is handled by any of the functions
**	below, otherwise it is ignored.
*/

int			input_read_ansi(t_vshdata *data)
{
	char	termcapbuf[TERMCAPBUFFSIZE];

	ft_bzero(termcapbuf, TERMCAPBUFFSIZE);
	if (data->input->c == '\e')
	{
		termcapbuf[0] = '\e';
		if (read(STDIN_FILENO, &termcapbuf[1], TERMCAPBUFFSIZE - 1) == -1)
			return (FUNCT_ERROR);
		if (ft_strequ(termcapbuf, TC_LEFT_ARROW) == true)
			curs_move_left(data);
		else if (ft_strequ(termcapbuf, TC_RIGHT_ARROW) == true)
			curs_move_right(data);
		else if (ft_strequ(termcapbuf, TC_HOME) == true)
			curs_go_home(data);
		else if (ft_strequ(termcapbuf, TC_END) == true)
			curs_go_end(data);
		else if (ft_strequ(termcapbuf, TC_DELETE) == true)
			input_handle_delete(data);
		else if (ft_strequ(termcapbuf, TC_UP_ARROW) == true)
			history_change_line(data, ARROW_UP);
		else if (ft_strequ(termcapbuf, TC_DOWN_ARROW) == true)
			history_change_line(data, ARROW_DOWN);
		else if (ft_strequ(termcapbuf, TC_CTRL_RIGHT_ARROW) == true)
			curs_move_next_word(data);
		else if (ft_strequ(termcapbuf, TC_CTRL_LEFT_ARROW) == true)
			curs_move_prev_word(data);
		else if (ft_strequ(termcapbuf, TC_CTRL_UP_ARROW) == true)
			curs_move_up(data);
		else if (ft_strequ(termcapbuf, TC_CTRL_DOWN_ARROW) == true)
			curs_move_down(data);
		else
		{
			#ifdef DEBUG
			ft_eprintf(">%s< TERMCAP NOT FOUND\n", &termcapbuf[1]); // DEBUG PRINT
			#endif
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
	if (data->input->c == INPUT_BACKSPACE)
		input_handle_backspace(data);
	else if (data->input->c == INPUT_CTRL_D)
		input_parse_ctrl_d(data);
	else if (data->input->c == INPUT_CTRL_K)
		input_parse_ctrl_k(data);
	else if (data->input->c == INPUT_CTRL_U)
		input_parse_ctrl_u(data);
	else if (data->input->c == INPUT_CTRL_Y)
		input_parse_ctrl_y(data);
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

int		input_resize_window_check(t_vshdata *data)
{
	struct winsize	new;
	int				newlines;
	char			*tc_clear_lines_str;
	unsigned		saved_index;
	int				extra;

	ioctl(STDIN_FILENO, TIOCGWINSZ, &new); // needs check
	data->curs->cur_ws_row = new.ws_row; // resizing shouldn't matter
	if (data->curs->cur_ws_col == UNINIT)
		data->curs->cur_ws_col = new.ws_col;
	else if (data->curs->cur_ws_col != new.ws_col)
	{
		saved_index = data->line->index; //save index
		newlines = data->curs->coords.y - 1;
		extra = 0;
		if (data->curs->cur_ws_col % new.ws_col > 0)
			extra = 1;
		// sleep(1);
		if (data->curs->coords.x - 1 > 0)
			ft_printf("\e[%iD", data->curs->coords.x - 1);
		ft_eprintf("NEWLINES: %i\n", newlines);
		if (data->curs->cur_ws_col > new.ws_col)
			newlines *= ((data->curs->cur_ws_col / new.ws_col) + extra);
		if (newlines > 0)
			ft_printf("\e[%iA", newlines);
		ft_eprintf("NEWLINES2: %i\n", newlines);
		// sleep(1);
		tc_clear_lines_str = tgetstr("cd", NULL);
		if (tc_clear_lines_str == NULL)
		{
			#ifdef DEBUG
			ft_eprintf("ERROR\n"); // needs proper message
			#endif
			return (FUNCT_ERROR); // do fatal shit
		}
		tputs(tc_clear_lines_str, 1, &ft_tputchar);
		// sleep(1);
		data->curs->coords.x = 1;
		data->curs->coords.y = 1;
		data->curs->cur_ws_col = new.ws_col;
		shell_display_prompt(data, data->prompt->cur_prompt_type);
		// sleep(1);
		data->line->index = data->line->len_cur;
		input_print_str(data, data->line->line);
		// sleep(1);
		data->line->index = data->line->len_cur;
		curs_go_home(data);
		// sleep(1);
		curs_move_n_right(data, saved_index);
		// sleep(1);
	}
	return (FUNCT_SUCCESS);
}

static int	reset_input_read_return(t_vshdata *data, int ret)
{
	data->input->c = '\0';
	data->line->index = 0;
	data->line->len_max = 64;
	data->line->len_cur = 0;
	data->curs->coords.x = 1;
	data->curs->coords.y = 1;
	//probably more shit?
	return (ret);
}

int			input_read(t_vshdata *data)
{
	if (data == NULL)
		return (FUNCT_ERROR);
	data->line->line = ft_strnew(data->line->len_max);
	if (data->line->line == NULL)
		return (reset_input_read_return(data, FUNCT_ERROR));
	while (true)
	{
		input_resize_window_check(data);
		if (read(STDIN_FILENO, &data->input->c, 1) == -1)
			return (reset_input_read_return(data, FUNCT_ERROR));
		if (input_parse_ctrl_c(data) == FUNCT_SUCCESS)
			return (reset_input_read_return(data, NEW_PROMPT));
		else if (input_read_ansi(data) == FUNCT_FAILURE)
		{
			if (input_read_special(data) == FUNCT_FAILURE)
			{
				if (input_parse_char(data) == FUNCT_ERROR)
					return (reset_input_read_return(data, FUNCT_ERROR));
				if (data->input->c == '\n')
				{
					curs_go_end(data);
					break ;
				}
			}
		}
		data->input->c = '\0';
	}
	return (reset_input_read_return(data, FUNCT_SUCCESS));
}

/*
int		input_resize_window_check(t_vshdata *data)
{
	struct winsize	new;
	int				newlines;
	char			*tc_clear_lines_str;
	unsigned		saved_index;
	int				extra;
	// int				random_int_lmao;
	// int				another_random_int;

	ioctl(STDIN_FILENO, TIOCGWINSZ, &new); // needs check
	data->curs->cur_ws_row = new.ws_row; // resizing shouldn't matter
	if (data->curs->cur_ws_col == UNINIT)
		data->curs->cur_ws_col = new.ws_col;
	else if (data->curs->cur_ws_col != new.ws_col)
	{
		sleep(100);
		saved_index = data->line->index; //save index
		newlines = data->curs->coords.y - 1;
		extra = 0;
		if (data->curs->cur_ws_col % new.ws_col > 0)
			extra = 1;
		// sleep(1);
		if (data->curs->coords.x - 1 > 0)
			ft_printf("\e[%iD", data->curs->coords.x - 1);
		ft_eprintf("NEWLINES: %i\n", newlines);
		if (data->curs->cur_ws_col > new.ws_col)
			newlines *= data->curs->cur_ws_col / new.ws_col + extra;
		
		// random_int_lmao = data->curs->cur_ws_col / new.ws_col + extra;
		// if (data->curs->cur_ws_col > new.ws_col && random_int_lmao > 0)
		// {
		// 		char	*tc_scroll_up_str;
		// 		// ft_printf("\e[%iD", data->curs->coords.x - 1);
		// 		tc_scroll_up_str = tgetstr("sr", NULL);
		// 		if (tc_scroll_up_str == NULL)
		// 		{
		// 			ft_eprintf("ERROR\n"); // needs proper message
		// 			return (FUNCT_ERROR); // do fatal shit
		// 		}
		// 		while (random_int_lmao > 0)
		// 		{
		// 			ft_eprintf("GOES UP ONCE\n");
		// 			another_random_int = get_curs_row(data) - 1;
		// 			// sleep(1);
		// 			ft_printf("\e[%iD", data->curs->coords.x - 1);
		// 			// sleep(1);
		// 			ft_printf("\e[%iA", another_random_int);
		// 			// sleep(1);
		// 			tputs(tc_scroll_up_str, 1, &ft_tputchar);
		// 			// sleep(1);
		// 			ft_printf("\e[%iB", another_random_int + 1);
		// 			// sleep(1);
		// 			ft_printf("\e[%iC", data->curs->coords.x - 1);
		// 			// sleep(1);
		// 			random_int_lmao--;
		// 		}
		// 		// ft_printf("\e[%iC", data->curs->coords.x - 1);
		// }
		// ft_printf("\e[%iD", data->curs->coords.x - 1);
		if (newlines > 0)
			ft_printf("\e[%iA", newlines);
		// sleep(1);
		ft_eprintf("NEWLINES2: %i\n", newlines);
		// sleep(1);
		tc_clear_lines_str = tgetstr("cd", NULL);
		if (tc_clear_lines_str == NULL)
		{
			#ifdef DEBUG
			ft_eprintf("ERROR\n"); // needs proper message
			#endif
			return (FUNCT_ERROR); // do fatal shit
		}
		// sleep(1);
		tputs(tc_clear_lines_str, 1, &ft_tputchar);
		// sleep(1);
		data->curs->coords.x = 1;
		data->curs->coords.y = 1;
		data->curs->cur_ws_col = new.ws_col;
		shell_display_prompt(data, data->prompt->cur_prompt_type);
		// sleep(1);
		data->line->index = data->line->len_cur;
		input_print_str(data, data->line->line);
		// sleep(1);
		data->line->index = data->line->len_cur;
		curs_go_home(data);
		curs_move_n_right(data, saved_index);
	}
	return (FUNCT_SUCCESS);
}
 */