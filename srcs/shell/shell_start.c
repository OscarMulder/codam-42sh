/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   shell_start.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: omulder <omulder@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/04/18 16:44:50 by omulder        #+#    #+#                */
/*   Updated: 2019/05/22 12:12:12 by mavan-he      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"

void	tokenlstiter(t_tokenlst *token_lst, void (*f)(t_tokenlst *elem))
{
	if (token_lst == NULL || f == NULL)
		return ;
	f(token_lst);
	tokenlstiter(token_lst->next, f);
}

int		shell_start(void)
{
	int			status;
	char		*line;
	t_tokenlst	*token_lst;

	status = 1;
	line = NULL;
	token_lst = NULL;
	while (status != CTRLD)
	{
		shell_display_prompt();
		status = input_read(&line);
		if (lexer(line, &token_lst) != FUNCT_SUCCESS)
		{
			ft_strdel(&line);
			continue ;
		}
		#ifdef DEBUG
		ft_printf("\n>>>> LINE <<<<\n%s\n\n>>>> TOKEN_LST <<<<\n", line);
		tokenlstiter(token_lst, print_node);
		#endif
		/* ADD EXPANSION FUNC ? */
		parser(&token_lst);
		/* ADD EVALUATOR */
		/* ADD AST DEL */
		tokenlstdel(&token_lst);
		ft_strdel(&line);
		ft_putendl("");
	}
	return (FUNCT_SUCCESS);
}
