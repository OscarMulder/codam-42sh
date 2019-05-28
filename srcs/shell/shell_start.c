/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   shell_start.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: omulder <omulder@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/04/18 16:44:50 by omulder        #+#    #+#                */
/*   Updated: 2019/05/28 12:02:28 by jbrinksm      ########   odam.nl         */
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

int		shell_read_till_stop(char **heredoc, char *stop)
{
	char		c;
	unsigned	index;
	int			status;
	int			input_state;

	index = 0;
	input_state = 0;
	*heredoc = ft_strnew(0);
	while (read(STDIN_FILENO, &c, 1) > 0)
	{
		if (ft_strcmp(&heredoc[index], stop) == 0)
			break ;
		status = 0;
		status |= input_parse_escape(c, &input_state);
		status |= input_parse_home(c, &input_state, &index);
		status |= input_parse_end(c, &input_state, &index, heredoc);
		status |= input_parse_prev(c, &input_state, &index, heredoc);
		status |= input_parse_next(c, &input_state, &index, heredoc);
		status |= input_parse_delete(c, &input_state, &index, heredoc);
		status |= input_parse_ctrl_up(c, &input_state, &index, heredoc);
		status |= input_parse_ctrl_down(c, &input_state, &index, heredoc);
		if (status == 0)
			input_state = 0;
		status |= input_parse_backspace(c, &index, heredoc);
		status |= input_parse_ctrl_d(c, &index, heredoc);
		status |= input_parse_ctrl_k(c, &index, heredoc);
		if (status == 0 &&
			input_parse_char(c, &index, heredoc) == FUNCT_FAILURE)
			return (FUNCT_FAILURE);
	}
	return (status);
}

void	shell_dless_input(t_tokenlst *token_lst)
{
	char 		*heredoc;
	t_tokenlst	*probe;
	char		*stop;

	probe = token_lst;
	while (probe != NULL)
	{
		if (probe->type == SLESS)
		{
			probe = probe->next;
			stop = ft_strdup(probe->value);
			ft_strdel(&(probe->value));
			read_till_stop(&heredoc, stop);
		}
		probe = probe->next;
	}
}

int		shell_start(void)
{
	int			status;
	char		*line;
	t_tokenlst	*token_lst;
	t_ast		*ast;

	status = 1;
	line = NULL;
	token_lst = NULL;
	ast = NULL;
	while (status != CTRLD)
	{
		shell_display_prompt();
		status = input_read(&line);
		#ifdef DEBUG
		ft_printf("\n>>>> LINE <<<<\n%s\n\n>>>> TOKEN_LST <<<<\n", line);
		#endif
		if (lexer(&line, &token_lst) != FUNCT_SUCCESS)
			continue ;
		#ifdef DEBUG
 		tokenlstiter(token_lst, print_node);
		#endif
		dless_input(token_lst);
		if (parser_start(&token_lst, &ast) != FUNCT_SUCCESS)
			continue ;
		#ifdef DEBUG
		print_tree(ast);
		#endif
		parser_astdel(&ast);
		/* ADD EVALUATOR */
		ft_putchar('\n');
	}
	return (FUNCT_SUCCESS);
}
