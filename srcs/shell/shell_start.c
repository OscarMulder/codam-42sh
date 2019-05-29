/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   shell_start.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: omulder <omulder@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/04/18 16:44:50 by omulder        #+#    #+#                */
/*   Updated: 2019/05/29 17:28:39 by jbrinksm      ########   odam.nl         */
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

int		shell_read_till_stop(char **heredoc, char *stop)
{
	char		c;
	unsigned	index;
	int			status;
	int			status2;
	int			input_state;
	char		*curr_line;

	status2 = 1;
	while (status2 != 0)
	{
		index = 0;
		input_state = 0;
		curr_line = ft_strnew(0);
		ft_putstr("\nheredoc > ");
		while (read(STDIN_FILENO, &c, 1) > 0)
		{
			if (c == '\n')
				break ;
			status = 0;
			status |= input_parse_escape(c, &input_state);
			status |= input_parse_home(c, &input_state, &index);
			status |= input_parse_end(c, &input_state, &index, &curr_line);
			status |= input_parse_prev(c, &input_state, &index, &curr_line);
			status |= input_parse_next(c, &input_state, &index, &curr_line);
			status |= input_parse_delete(c, &input_state, &index, &curr_line);
			status |= input_parse_ctrl_up(c, &input_state, &index, &curr_line);
			status |= input_parse_ctrl_down(c, &input_state, &index, &curr_line);
			if (status == 0)
				input_state = 0;
			status |= input_parse_backspace(c, &index, &curr_line);
			status |= input_parse_ctrl_d(c, &index, &curr_line);
			status |= input_parse_ctrl_k(c, &index, &curr_line);
			if (status == 0 &&
				input_parse_char(c, &index, &curr_line) == FUNCT_FAILURE)
				return (FUNCT_FAILURE);
		}
		status2 = ft_strcmp(curr_line, stop);
		if (status2)
		{
			if (*heredoc == NULL)
				*heredoc = ft_strdup(curr_line);
			else
				*heredoc = ft_strjoinfree(*heredoc, curr_line, 1);
		}
		ft_strdel(&curr_line);
	}
	return (status);
}

void	shell_dless_input(t_tokenlst *token_lst)
{
	char 		*heredoc;
	t_tokenlst	*probe;
	char		*stop;

	probe = token_lst;
	heredoc = NULL;
	while (probe != NULL)
	{
		if (probe->type == DLESS)
		{
			probe = probe->next;
			stop = ft_strdup(probe->value);
			ft_strdel(&(probe->value));
			shell_read_till_stop(&heredoc, stop);
			probe->value = ft_strdup(heredoc);
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
 		lexer_tokenlstiter(token_lst, print_node);
		#endif
		shell_dless_input(token_lst);
 		lexer_tokenlstiter(token_lst, print_node);
		if (parser_start(&token_lst, &ast) != FUNCT_SUCCESS)
			continue ;
		#ifdef DEBUG
		print_tree(ast);
		#endif
		parser_astdel(&ast);
		/* ADD EVALUATOR */
		/* ADD EXPANSION FUNC ? */
		lexer_tokenlstdel(&token_lst);
		ft_strdel(&line);
		ft_putendl("");
	}
	return (FUNCT_SUCCESS);
}
