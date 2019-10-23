/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   input_parse_special.c                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: rkuijper <rkuijper@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/08/30 14:05:27 by rkuijper       #+#    #+#                */
/*   Updated: 2019/10/23 15:08:28 by omulder       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"

int			input_parse_special(t_vshdata *data)
{
	if (data->input->c == INPUT_BACKSPACE)
		input_handle_backspace(data);
	else if (data->input->c == INPUT_CTRL_A)
		curs_go_home(data);
	else if (data->input->c == INPUT_CTRL_D)
		input_parse_ctrl_d(data);
	else if (data->input->c == INPUT_CTRL_E)
		curs_go_end(data);
	else if (data->input->c == INPUT_CTRL_K)
		input_parse_ctrl_k(data);
	else if (data->input->c == INPUT_CTRL_U)
		input_parse_ctrl_u(data);
	else if (data->input->c == INPUT_CTRL_Y)
		input_parse_ctrl_y(data);
	else if (data->input->c == INPUT_TAB)
		input_parse_tab(data);
	else if (data->input->c == INPUT_CTRL_R)
		input_parse_ctrl_r(data);
	else
		return (FUNCT_FAILURE);
	return (FUNCT_SUCCESS);
}
