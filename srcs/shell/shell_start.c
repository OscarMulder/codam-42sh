/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   shell_start.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: omulder <omulder@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/04/18 16:44:50 by omulder        #+#    #+#                */
/*   Updated: 2019/07/15 16:55:02 by omulder       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"

void	lexer_tokenlstiter(t_tokenlst *lst, void (*f)(t_tokenlst *elem))
{
	if (lst == NULL || f == NULL)
		return ;
	f(lst);
	lexer_tokenlstiter(lst->next, f);
}

int		shell_start(t_envlst *envlst)
{
	int			status;
	int			exit_code;
	char		*line;
	t_tokenlst	*token_lst;
	t_ast		*ast;
	t_history	**history;

	exit_code = EXIT_SUCCESS;
	status = 1;
	line = NULL;
	token_lst = NULL;
	ast = NULL;
	history_get_file_content(&history);
	while (status != CTRLD)
	{
		shell_display_prompt();
		status = input_read(&line, history);
		while (shell_quote_checker(&line, history) != FUNCT_SUCCESS)
			continue ;
		ft_putchar('\n');
		#ifdef DEBUG
		ft_printf("\n>>>> LINE <<<<\n%s\n\n>>>> TOKEN_LST <<<<\n", line);
		#endif
		if (lexer(&line, &token_lst) != FUNCT_SUCCESS)
			continue ;
		if (shell_dless_input(token_lst, history) != FUNCT_SUCCESS)
			continue ;
		#ifdef DEBUG
 		lexer_tokenlstiter(token_lst, print_node);
		#endif
		if (parser_start(&token_lst, &ast) != FUNCT_SUCCESS)
			continue ;
		#ifdef DEBUG
		print_tree(ast);
		#endif
		exec_start(ast, envlst, &exit_code, 0);
		parser_astdel(&ast);
		/* ADD EVALUATOR */
		/* ADD EXPANSION FUNC ? */
		lexer_tokenlstdel(&token_lst);
		ft_strdel(&line);
	}
	return (FUNCT_SUCCESS);
}
