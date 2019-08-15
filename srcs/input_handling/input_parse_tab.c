/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   input_parse_tab.c                                  :+:    :+:            */
/*                                                     +:+                    */
/*   By: omulder <omulder@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/08/15 13:57:43 by omulder        #+#    #+#                */
/*   Updated: 2019/08/15 15:52:50 by omulder       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"

int		input_parse_tab(t_inputdata *data, t_vshdata *vshdata)
{
	auto_start(vshdata, data);
	#ifdef DEBUG
	ft_eprintf("LINE: %s", vshdata->line);
	#endif
	return (FUNCT_SUCCESS);
}
