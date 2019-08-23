/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   input_parse_ctrl_y.c                               :+:    :+:            */
/*                                                     +:+                    */
/*   By: rkuijper <rkuijper@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/08/23 13:05:41 by rkuijper       #+#    #+#                */
/*   Updated: 2019/08/23 14:49:11 by rkuijper      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <vsh.h>

static void	str_insert_str(t_inputdata *data, char *dst,
	char *src, unsigned src_len)
{
	ft_strncpy(&dst[data->index + src_len],
		&dst[data->index], data->len_cur - data->index);
	ft_strncpy(&dst[data->index], src, src_len);
}

static int	line_insert_copy(t_inputdata *data, t_vshdata *vshdata,
	unsigned copy_len)
{
	char		*tmp;

	if (data->len_cur + copy_len < data->len_max)
		str_insert_str(data, vshdata->line, vshdata->line_copy, copy_len);
	else
	{
		data->len_max += copy_len;
		tmp = ft_strnew(data->len_max);
		if (tmp == NULL)
			return (FUNCT_ERROR);
		ft_strcpy(tmp, vshdata->line);
		ft_strdel(&vshdata->line);
		str_insert_str(data, tmp, vshdata->line_copy, copy_len);
		vshdata->line = tmp;
	}
	data->len_cur += copy_len;
	return (FUNCT_SUCCESS);
}

void		input_parse_ctrl_y(t_inputdata *data, t_vshdata *vshdata)
{
	unsigned	copy_len;
	unsigned	old_index;
	
	if (vshdata->line_copy != NULL)
	{
		old_index = data->index;
		copy_len = ft_strlen(vshdata->line_copy);
		if (line_insert_copy(data, vshdata, copy_len) == FUNCT_SUCCESS)
		{
			input_print_str(data, &vshdata->line[data->index]);
			data->index = data->len_cur;
			curs_move_n_left(data, data->index - old_index - copy_len);
		}
	}
}
