/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   auto_lst_print.c                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: mavan-he <mavan-he@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/08/13 19:53:22 by mavan-he       #+#    #+#                */
/*   Updated: 2019/08/15 10:44:43 by omulder       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"
#include <sys/ioctl.h>

static int			get_row(int n, int width, int length)
{
	int row;

	if (n == 0 || width == 0 || length == 0)
		return (1);
	if (width / length == 0)
		return (1);
	row = (n / (width / length));
	if ((n % (width / length)) != 0)
		row++;
	return (row);
}

static int			get_termwidth(void)
{
	struct winsize w;

	ioctl(0, TIOCGWINSZ, &w);
	return (w.ws_col);
}

static t_list		*set_ptr(t_list *ptr, t_list *matchlst, int l, int row)
{
	int	j;

	j = 0;
	while (j < row)
	{
		if (ptr->next != NULL)
			ptr = ptr->next;
		else
		{
			ptr = matchlst;
			j = 0;
			while (j < l)
			{
				ptr = ptr->next;
				j++;
			}
			break ;
		}
		j++;
	}
	return (ptr);
}

static t_print		init_vars(t_list *matchlst, int lst_len)
{
	t_print		a;

	a.length = auto_lenname(matchlst, 0) + 1;
	a.coli = 1;
	a.total = lst_len;
	a.row = get_row(a.total, get_termwidth(), a.length);
	if (a.row == 0)
		a.col = 1;
	else
		a.col = a.total / a.row;
	a.left = a.total % a.row;
	a.printed = 0;
	a.extra = 0;
	return (a);
}

void				auto_lst_print(t_list *matchlst, int lst_len)
{
	t_list		*ptr;
	t_print		a;

	auto_sort_n(&matchlst);
	ptr = matchlst;
	a = init_vars(matchlst, lst_len);
	while (a.printed < a.total)
	{
		if (a.coli == a.col && a.extra == (a.row - 1))
			ft_printf("%s\n", ptr->content);
		else if (a.coli == (a.col + (a.left != 0)))
		{
			ft_printf("%s\n", ptr->content);
			if (a.left != 0)
				a.left--;
			a.coli = 0;
			a.extra++;
		}
		else
			ft_printf("%-*s", a.length, ptr->content);
		a.coli++;
		ptr = set_ptr(ptr, matchlst, a.extra, a.row);
		a.printed++;
	}
}
