/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   input_print_str.c                                  :+:    :+:            */
/*                                                     +:+                    */
/*   By: rkuijper <rkuijper@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/08/23 11:54:27 by rkuijper       #+#    #+#                */
/*   Updated: 2019/08/29 10:44:43 by jbrinksm      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <vsh.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <term.h>

int	get_curs_row(t_vshdata *data)
{
	char	*buf;
	int		ret;
	int		i;
	int		row;

	i = 0;
	data->term->termios_p->c_cc[VMIN] = 5;
	data->term->termios_p->c_cc[VTIME] = 0;
	ret = tcsetattr(STDIN_FILENO, TCSANOW, data->term->termios_p);
	if (ret == -1)
	{
		// ft_eprintf(E_TERM_CNT_GET); <--- WRONG ERROR MESSAGE
		return (FUNCT_ERROR);
	}
	buf = ft_strnew(TC_MAXRESPONSESIZE);
	if (buf == NULL)
	{
		data->term->termios_p->c_cc[VMIN] = 0;
		data->term->termios_p->c_cc[VTIME] = 2;
		ret = tcsetattr(STDIN_FILENO, TCSANOW, data->term->termios_p);
		return (FUNCT_ERROR);
	}
	ft_putstr("\e[6n");
	ret = read(STDIN_FILENO, buf, TC_MAXRESPONSESIZE);
	if (ret == -1)
	{
		data->term->termios_p->c_cc[VMIN] = 0;
		data->term->termios_p->c_cc[VTIME] = 2;
		ret = tcsetattr(STDIN_FILENO, TCSANOW, data->term->termios_p);
		return (ft_free_return(buf, FUNCT_ERROR));
	}
	while (buf[i] != '[' && buf[i] != '\0')
		i++;
	if (buf[i] == '[')
		i++;
	if (ft_isdigit(buf[i]) == false)
	{
		data->term->termios_p->c_cc[VMIN] = 0;
		data->term->termios_p->c_cc[VTIME] = 2;
		ret = tcsetattr(STDIN_FILENO, TCSANOW, data->term->termios_p);
		return (ft_free_return(buf, FUNCT_ERROR));
	}
	row = ft_atoi(&buf[i]);
	data->term->termios_p->c_cc[VMIN] = 0;
	data->term->termios_p->c_cc[VTIME] = 2;
	ret = tcsetattr(STDIN_FILENO, TCSANOW, data->term->termios_p);
	if (ret == -1)
	{
		// ft_eprintf(E_TERM_CNT_GET); <--- WRONG ERROR MESSAGE
		return (ft_free_return(buf, FUNCT_ERROR));
	}
	ft_strdel(&buf);
	return (row);
}

// STILL NEEDS TO RETURN SHIT WHEN IT FAILS
static void	print_str(t_vshdata *data, unsigned short maxcol, char *str)
{
	int		i;
	char	*tc_scroll_down_str;

	i = 0;
	while (str[i] != '\0')
	{
		ft_putchar(str[i]);
		if (str[i] == '\n')
		{
			data->curs->coords.x = 1;
			data->curs->coords.y++;
		}
		i++;
		data->curs->coords.x++;
		if (data->curs->coords.x > maxcol)
		{
			if (get_curs_row(data) == data->curs->cur_ws_row)
			{
				ft_printf("\e[%iD", data->curs->coords.x - 1);
				tc_scroll_down_str = tgetstr("sf", NULL);
				if (tc_scroll_down_str == NULL)
				{
					ft_eprintf("ERROR\n"); // needs proper message
					return ; // do fatal shit
				}
				tputs(tc_scroll_down_str, 1, &ft_tputchar);
				ft_strdel(&tc_scroll_down_str);
				ft_printf("\e[%iC", data->curs->coords.x - 1);
			}
			ft_printf("\e[B\e[%iD", maxcol);
			data->curs->coords.x = 1;
			data->curs->coords.y++;
		}
	}
}

void	input_print_str(t_vshdata *data, char *str)
{
	print_str(data, data->curs->cur_ws_col, str);
}
