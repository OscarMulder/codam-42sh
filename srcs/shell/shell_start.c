/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   shell_start.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: omulder <omulder@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/04/18 16:44:50 by omulder        #+#    #+#                */
/*   Updated: 2019/05/13 18:52:25 by mavan-he      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"

#include "vsh.h"

int		shell_start(void)
{
	int			status;
	char		*line;
	t_list		*token_lst;

	status = 1;
	line = NULL;
	token_lst = NULL;
	while (status != CTRLD)
	{
		shell_display_prompt();
		status = input_read(&line);
		token_lst = lexer(line);
		if (token_lst == NULL)
		{
			ft_strdel(&line);
			continue ;
		}
		#ifdef DEBUG
		ft_printf("Line: \n%s\n\n", line);
		ft_putstr("Token lst:\n");
		//ft_lstiter(lst, print_content); ADD PRINT LST FUNC
		#endif
		/* ADD EXPANSION FUNC ? */
		/* ADD PARSER */
		/* ADD EVALUATOR */
		/* ADD LST DEL */
		/* ADD AST DEL */
		ft_strdel(&line);
		ft_putendl("");
	}
	return (FUNCT_SUCCESS);
}
