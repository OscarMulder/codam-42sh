/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   auto_lst_print.c                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: mavan-he <mavan-he@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/08/13 19:53:22 by mavan-he       #+#    #+#                */
/*   Updated: 2019/08/13 20:07:03 by mavan-he      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"

// Need to make this print properly ofcourse

void	auto_lst_print(t_list *matchlst)
{
	while (matchlst != NULL)
	{
		ft_putstr((char *)matchlst->content);
		ft_putstr("    ");
		matchlst = matchlst->next;
	}
	ft_putchar('\n');
}
