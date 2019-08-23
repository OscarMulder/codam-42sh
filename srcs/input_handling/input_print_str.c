/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   input_print_str.c                                  :+:    :+:            */
/*                                                     +:+                    */
/*   By: rkuijper <rkuijper@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/08/23 11:54:27 by rkuijper       #+#    #+#                */
/*   Updated: 2019/08/23 11:56:15 by rkuijper      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <vsh.h>
#include <sys/ioctl.h>

void	input_print_str(t_inputdata *data, char *str)
{
	int				i;
	struct winsize	ws;

	i = 0;
	ioctl(STDIN_FILENO, TIOCGWINSZ, &ws);
	while (str[i] != '\0')
	{
		ft_putchar(str[i]);
		data->coords.x++;
		if (data->coords.x > ws.ws_col)
		{
			ft_putchar('\n');
			data->coords.y++;
			data->coords.x = 1;
		}
		i++;
	}
}
