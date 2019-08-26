/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   input_print_str.c                                  :+:    :+:            */
/*                                                     +:+                    */
/*   By: rkuijper <rkuijper@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/08/23 11:54:27 by rkuijper       #+#    #+#                */
/*   Updated: 2019/08/26 19:38:08 by jbrinksm      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <vsh.h>
#include <sys/ioctl.h>

static int	get_total_newlines(t_vshdata *data, unsigned short maxcol, char *str)
{
	int	total_newlines;
	int x_copy;
	int	i;

	total_newlines = 0;
	x_copy = data->curs->coords.x;
	i = 0;
	while (str[i] != '\0')
	{
		x_copy++;
		if (x_copy > maxcol || str[i] == '\n')
		{
			total_newlines++;
			x_copy = 1;
		}
		i++;
	}
	return (total_newlines);
}

static void	fill_strbuf(t_vshdata *data, unsigned short maxcol, char **strbuf, char *str)
{
	int str_i;
	int	strbuf_i;

	str_i = 0;
	strbuf_i = 0;
	while (str[str_i] != '\0')
	{
		(*strbuf)[strbuf_i] = str[str_i];
		if (str[str_i] == '\n')
		{
			data->curs->coords.x = 0;
			data->curs->coords.y++;
		}
		str_i++;
		strbuf_i++;
		data->curs->coords.x++;
		ft_eprintf("%i %i\n", data->curs->coords.x, maxcol);
		if (data->curs->coords.x > maxcol)
		{
			// (*strbuf)[strbuf_i] = '\n';
			strbuf_i++;
			data->curs->coords.y++;
			data->curs->coords.x = 1;
		}
	}
}

// STILL NEEDS TO RETURN SHIT WHEN IT FAILS

void	input_print_str(t_vshdata *data, char *str)
{
	int				i;
	char			*strbuf;
	int				total_newlines;

	i = 0;
	total_newlines = get_total_newlines(data, data->curs->cur_ws_col, str);
	strbuf = ft_strnew(ft_strlen(str) + total_newlines);
	if (strbuf == NULL)
		return ; // GO CRAZY <-----------------
	fill_strbuf(data, data->curs->cur_ws_col, &strbuf, str);
	ft_putstr(strbuf);
	ft_strdel(&strbuf);
}
