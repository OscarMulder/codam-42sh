/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   input_parse_end.c                                  :+:    :+:            */
/*                                                     +:+                    */
/*   By: rkuijper <rkuijper@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/05/16 13:39:04 by rkuijper       #+#    #+#                */
/*   Updated: 2019/08/08 11:42:17 by jbrinksm      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"
#include <sys/ioctl.h>

// int	input_parse_end(t_inputdata *data, char **line)
// {
// 	if ((data->input_state == INPUT_BRACE && data->c == 'F') || data->c == '\5')
// 	{
// 		ft_printf("%s", *line + data->index);
// 		data->index = ft_strlen(*line);
// 		data->input_state = INPUT_NONE;
// 		return (FUNCT_SUCCESS);
// 	}
// 	return (FUNCT_FAILURE);
// }

int			curs_go_end(t_inputdata *data, t_vshdata *vshdata)
{
	size_t	index_to_end;

	index_to_end = ft_strlen(vshdata->line) - data->index;
	if (index_to_end > 0)
		curs_move_n_right(data, vshdata, index_to_end);
	return (FUNCT_SUCCESS);
}
