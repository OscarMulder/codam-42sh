/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   history_line_to_array.c                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: mavan-he <mavan-he@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/05/30 18:55:25 by mavan-he       #+#    #+#                */
/*   Updated: 2019/05/30 20:10:00 by mavan-he      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"

int		history_line_to_array(char *line)
{
	history[history_i % 500] = ft_strdup(line);
	if (history[history_i % 500] == NULL)
		return (FUNCT_ERROR);
	history_i++;
	return (FUNCT_SUCCESS);
}
