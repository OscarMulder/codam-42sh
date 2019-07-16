/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   shell_start.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: omulder <omulder@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/04/18 16:44:50 by omulder        #+#    #+#                */
/*   Updated: 2019/07/16 22:59:24 by jbrinksm      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"
#include <unistd.h> //REMOVE

void	lexer_tokenlstiter(t_tokenlst *lst, void (*f)(t_tokenlst *elem))
{
	if (lst == NULL || f == NULL)
		return ;
	f(lst);
	lexer_tokenlstiter(lst->next, f);
}

t_pipes	*init_pipestruct(void)
{
	t_pipes	*pipes;

	pipes = ft_memalloc(sizeof(t_pipes));
	if (pipes == NULL)
		return (NULL);
	pipes->parentpipe = ft_memalloc(sizeof(int) * 2);
	if (pipes->parentpipe == NULL)
		return (NULL);
	pipes->currentpipe = ft_memalloc(sizeof(int) * 2);
	if (pipes->currentpipe == NULL)
		return (NULL);
	pipes->fds.stdin = dup(STDIN_FILENO);
	pipes->fds.stdout = dup(STDOUT_FILENO);
	pipes->fds.stderr = dup(STDERR_FILENO);
	pipes->pipeside = 0;
	return (pipes);
}

int		shell_start(t_envlst *envlst)
{
	int			status;
	int			exit_code;
	char		*line;
	t_tokenlst	*token_lst;
	t_ast		*ast;
	t_pipes		*pipes;

	exit_code = EXIT_SUCCESS;
	status = 1;
	line = NULL;
	token_lst = NULL;
	ast = NULL;
	while (status != CTRLD)
	{
		shell_display_prompt();
		status = input_read(&line);
		while (shell_quote_checker(&line) != FUNCT_SUCCESS)
			continue ;
		ft_putchar('\n');
		history_line_to_array(line);
		#ifdef DEBUG
		ft_printf("\n>>>> LINE <<<<\n%s\n\n>>>> TOKEN_LST <<<<\n", line);
		#endif
		if (lexer(&line, &token_lst) != FUNCT_SUCCESS)
			continue ;
		if (shell_dless_input(token_lst) != FUNCT_SUCCESS)
			continue ;
		if ((token_lst->next)->type == NEWLINE)
		{
			lexer_tokenlstdel(&token_lst);
			continue ;
		}

		#ifdef DEBUG
 		lexer_tokenlstiter(token_lst, print_node);
		#endif
		if (parser_start(&token_lst, &ast) != FUNCT_SUCCESS)
			continue ;
		#ifdef DEBUG
		print_tree(ast);
		#endif

		pipes = init_pipestruct();
		if (pipes == NULL)
			continue ;
			
		exec_start(ast, envlst, &exit_code, pipes);
		parser_astdel(&ast);
		// free pipeshit
	}
	return (FUNCT_SUCCESS);
}
