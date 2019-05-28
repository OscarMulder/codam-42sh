/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   shell_start.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: omulder <omulder@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/04/18 16:44:50 by omulder        #+#    #+#                */
/*   Updated: 2019/05/28 19:17:54 by jbrinksm      ########   odam.nl         */
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

int			shell_isescaped(const char *line, int index)
{
	if (index == 0)
		return (FUNCT_FAILURE);
	else
	{
		if (line[index - 1] == '\\')
			return (FUNCT_SUCCESS);
		else
			return (FUNCT_FAILURE);
	}
}

char		*ft_chartostr(char c)
{
	char	*str;

	str = ft_strnew(2);
	str[0] = c;
	return (str);
}

char		shell_quote_checker_find_quote(char *line)
{
	int		index;
	char	quote;
	int		is_escaped;
	char	c;

	index = 0;
	quote = '\0';
	while (line[index] != '\0')
	{
		is_escaped = shell_isescaped(line, index);
		c = line[index];
		if (!quote && (c == '\'' || c == '"') && !is_escaped)
			quote = c;
		else if (quote && c == quote && !is_escaped)
			quote = '\0';
		index++;
	}
	return (quote);
}

char		*ft_joinstrcstr(char *s1, char c, char *s2)
{
	char	*str;

	str = ft_strnew(ft_strlen(s1) + 1 + ft_strlen(s2));
	if (str == NULL)
		return (NULL);
	str = ft_strcpy(str, s1);
	str = ft_strcat(str, ft_chartostr(c));
	str = ft_strcat(str, s2);
	return (str);
}

char		*ft_joinstrcstr_free_s1(char *s1, char c, char *s2)
{
	char	*str;

	str = ft_joinstrcstr(s1, c, s2);
	ft_strdel(&s1);
	return (str);
}

char		*ft_joinstrcstr_free_s2(char *s1, char c, char *s2)
{
	char	*str;

	str = ft_joinstrcstr(s1, c, s2);
	ft_strdel(&s2);
	return (str);
}

char		*ft_joinstrcstr_free_all(char *s1, char c, char *s2)
{
	char	*str;

	str = ft_joinstrcstr(s1, c, s2);
	ft_strdel(&s1);
	ft_strdel(&s2);
	return (str);
}

void		shell_quote_checker(char **line)
{
	char	quote;
	char	*extra_line;

	quote = shell_quote_checker_find_quote(*line);
	while (quote != '\0')
	{
		if (quote == '\'')
			ft_printf("\nquote> ", quote);
		else if (quote == '"')
			ft_printf("\ndquote> ", quote);
		input_read(&extra_line);
		*line = ft_joinstrcstr_free_all(*line, '\n', extra_line);
		quote = shell_quote_checker_find_quote(*line);
	}
}
int     shell_start(void)
{

	int         status;
	char        *line;
	t_tokenlst  *token_lst;
	t_ast       *ast;

	status = 1;
	line = NULL;
	token_lst = NULL;
	ast = NULL;
	while (status != CTRLD)
	{
		shell_display_prompt();
		status = input_read(&line);
		shell_quote_checker(&line);
		#ifdef DEBUG
		ft_printf("\n>>>> LINE <<<<\n%s\n\n>>>> TOKEN_LST <<<<\n", line);
		#endif
		if (lexer(line, &token_lst) != FUNCT_SUCCESS)
			continue ;
		#ifdef DEBUG
		lexer_tokenlstiter(token_lst, print_node);
		#endif
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