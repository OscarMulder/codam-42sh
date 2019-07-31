/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   shell_start.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: omulder <omulder@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/04/18 16:44:50 by omulder        #+#    #+#                */
/*   Updated: 2019/07/31 12:59:18 by jbrinksm      ########   odam.nl         */
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

static bool	is_last_char_escaped_newline(char *line)
{
	int i;

	i = 0;
	while (line[i] != '\0')
		i++;
	if (i == 0)
		return (false);
	i--;
	if (line[i] == '\n' && tools_is_char_escaped(line, i) == true)
		return (true);
	return (false);
}

static int	escaped_newlines(t_vshdata *vshdata, char **line, int *status)
{
	int		ret;
	char	*extra_line;

	ret = is_last_char_escaped_newline(*line);
	if (ret == false)
		return (FUNCT_FAILURE);
	while (ret != false)
	{
		ft_putstr("\nnewline> ");
		input_read(vshdata, &extra_line, status);
		*line = ft_strjoinfree_all(*line, extra_line);
		ret = is_last_char_escaped_newline(*line);
	}
	return (FUNCT_SUCCESS);
}

static int	handle_quotes_and_escapes(t_vshdata *vshdata, char **line, int *status)
{
	int ret;

	ret = FUNCT_SUCCESS;
	while (ret == FUNCT_SUCCESS)
	{
		ret = shell_quote_checker(vshdata, line, status);
		if (ret == FUNCT_ERROR)
			return (FUNCT_ERROR);
		ret = escaped_newlines(vshdata, line, status);
		if (ret == FUNCT_ERROR)
			return (FUNCT_ERROR);
	}
	return (FUNCT_SUCCESS);
}

int		shell_start(t_vshdata *vshdata)
{
	int			status;
	char		*line;
	t_tokenlst	*token_lst;
	t_ast		*ast;
	t_pipes		pipes;

	status = 1;
	line = NULL;
	token_lst = NULL;
	ast = NULL;
	pipes = redir_init_pipestruct();
	while (status != CTRLD)
	{
		parser_astdel(&ast);
		lexer_tokenlstdel(&token_lst);
		shell_display_prompt();
		if (input_read(vshdata, &line, &status) == FUNCT_ERROR)
			continue;
		if (handle_quotes_and_escapes(vshdata, &line, &status) == FUNCT_ERROR)
			continue ;
		ft_putchar('\n');
		if (history_line_to_array(vshdata->history, &line) == FUNCT_ERROR)
			continue ;
		#ifdef DEBUG
		ft_printf("\n>>>> LINE <<<<\n%s\n\n>>>> TOKEN_LST <<<<\n", line);
		#endif
		if (lexer(&line, &token_lst) != FUNCT_SUCCESS)
			continue ;
		if (shell_dless_input(vshdata, &token_lst) != FUNCT_SUCCESS)
			continue ;
		if (alias_expansion(vshdata, &token_lst, NULL) != FUNCT_SUCCESS)
			continue ;
		if ((token_lst->next)->type == NEWLINE)
			continue ;
		#ifdef DEBUG
 		lexer_tokenlstiter(token_lst, print_node);
		#endif
		if (parser_start(&token_lst, &ast) != FUNCT_SUCCESS)
			continue ;
		#ifdef DEBUG
		ft_putstr("\n\n\nTREE:\n\n");
		print_tree(ast);
		#endif
		exec_start(ast, vshdata, pipes);
	}
	return (FUNCT_SUCCESS);
}
