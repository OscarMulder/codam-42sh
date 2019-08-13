/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   auto_big_lst.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: mavan-he <mavan-he@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/08/13 20:07:40 by mavan-he       #+#    #+#                */
/*   Updated: 2019/08/13 20:11:39 by mavan-he      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"

int		auto_big_lst(t_list *matchlst, int lst_len)
{
	(void)matchlst;
	ft_printf("Display all %d possibilities? (y or n)", lst_len);
	return (FUNCT_SUCCESS);
}
