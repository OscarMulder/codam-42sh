/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   tools_isidentifierchar.c                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: jbrinksm <jbrinksm@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/07/13 19:50:31 by jbrinksm       #+#    #+#                */
/*   Updated: 2019/07/13 19:56:06 by jbrinksm      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"

bool	tools_isidentifierchar(char c)
{
	if (ft_isalnum(c) == true || c == '_')
		return (true);
	return (false);
}