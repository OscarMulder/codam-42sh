/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   auto_big_lst.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: mavan-he <mavan-he@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/08/13 20:07:40 by mavan-he       #+#    #+#                */
/*   Updated: 2019/08/14 15:47:10 by mavan-he      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"

int		auto_big_lst(t_list *matchlst, int lst_len)
{
	char c;

	ft_printf("Display all %d possibilities? (y or n)", lst_len);
	c = 0;
	while (c != 'y' && c != 'n')
	{
		if (read(STDIN_FILENO, &c, 1) == -1)
		{
			ft_eprintf("vsh: error reading input\n");
			return (FUNCT_ERROR);
		}
	}
	ft_putchar('\n');
	if (c == 'y')
		auto_lst_print(matchlst);
	else if (c == 'n')
		return (FUNCT_FAILURE);
	return (FUNCT_SUCCESS);
}
