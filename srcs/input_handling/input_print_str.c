/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   input_print_str.c                                  :+:    :+:            */
/*                                                     +:+                    */
/*   By: rkuijper <rkuijper@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/08/23 11:54:27 by rkuijper       #+#    #+#                */
/*   Updated: 2019/09/12 17:44:10 by rkuijper      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <vsh.h>
#include <termios.h>
#include <term.h>

int			get_curs_row(void)
{
	int		i;
	int		row;
	char	*buf;

	i = 0;
	buf = ft_strnew(TC_MAXRESPONSESIZE);
	if (buf == NULL)
		return (err_ret(E_ALLOC_STR));
	ft_putstr("\e[6n");
	if (read(STDIN_FILENO, buf, TC_MAXRESPONSESIZE) == -1)
		ft_free_return(buf, FUNCT_ERROR);
	while (buf[i] != '[' && buf[i] != '\0')
		i++;
	if (buf[i] == '[')
		i++;
	#ifdef DEBUG
	ft_eprintf("curs: <%s>\n", &buf[1]);
	#endif
	if (ft_isdigit(buf[i]) == false)
		return (ft_free_return(buf, FUNCT_ERROR));
	row = ft_atoi(&buf[i]);
	ft_strdel(&buf);
	return (row);
}

static void	scroll_down_terminal(t_vshdata *data)
{
	ft_printf("\e[%iD", data->curs->coords.x - 1);
	tputs(data->termcaps->tc_scroll_down_str, 1, &ft_tputchar);
	ft_printf("\e[%iC", data->curs->coords.x - 1);
}

static void	update_newline_coords(t_vshdata *data)
{
	data->curs->coords.x = 1;
	data->curs->cur_relative_y++;
}

void		input_print_str(t_vshdata *data, char *str)
{
	int		i;

	i = 0;
	while (str[i] != '\0')
	{
		ft_putchar(str[i]);
		data->curs->coords.x++;
		if (str[i] == '\n')
		{
			update_newline_coords(data);
			if (data->curs->coords.y < data->curs->cur_ws_row)
				data->curs->coords.y++;
		}
		if (data->curs->coords.x > data->curs->cur_ws_col)
		{
			if (data->curs->coords.y == data->curs->cur_ws_row)
				scroll_down_terminal(data);
			else
				data->curs->coords.y++;
			ft_printf("\e[B\e[%iD", data->curs->cur_ws_col);
			update_newline_coords(data);
		}
		i++;
	}
}
