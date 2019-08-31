/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   input_parse_ctrl_d.c                               :+:    :+:            */
/*                                                     +:+                    */
/*   By: omulder <omulder@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/05/16 13:46:55 by rkuijper       #+#    #+#                */
/*   Updated: 2019/08/29 11:59:04 by jbrinksm      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"

/*
**	Simply exits when line is empty, otherwise functions as `delete`.
*/

void	input_parse_ctrl_d(t_vshdata *data)
{
	if (data->input->c == INPUT_CTRL_D)
	{
		if (data->line->len_cur == 0)
			builtin_exit(NULL, data);
		else
			input_handle_delete(data);
	}
}
