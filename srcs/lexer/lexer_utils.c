/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   lexer_utils.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: mavan-he <mavan-he@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/05/16 10:23:43 by mavan-he       #+#    #+#                */
/*   Updated: 2019/05/21 19:47:36 by jbrinksm      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"

void	tokenlstdel(t_tokenlst **lst)
{
	if (*lst == NULL)
		return ;
	if ((*lst)->next != NULL)
		tokenlstdel(&(*lst)->next);
	if ((*lst)->type == WORD || (*lst)->type == ASSIGN)
		ft_strdel(&(*lst)->value.str);
	ft_memdel((void**)lst);
}

int		lexer_error(t_tokenlst **lst)
{
	if (*lst != NULL)
		tokenlstdel(lst);
	ft_putstr_fd("vsh: lexer: malloc error\n", STDERR_FILENO);
	return (FUNCT_ERROR);
}

void	change_state(t_scanner *scanner, void (*state_x)(t_scanner *scanner))
{
	(scanner->str_index)++;
	(scanner->tk_len)++;
	state_x(scanner);
}
