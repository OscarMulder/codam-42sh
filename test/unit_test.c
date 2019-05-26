/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   unit_test.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: jbrinksm <jbrinksm@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/04/18 16:37:32 by omulder        #+#    #+#                */
/*   Updated: 2019/05/26 15:34:40 by mavan-he      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

/*
**	General note: We have to make sure that we redirect the STDOUT to some sort
**	of file, so that we don't destroy the test_suite output.
**	(Also, see the comments at builtin_echo.)
*/

#include "vsh.h"
#include <criterion/criterion.h>
#include <criterion/redirect.h>

void redirect_all_stdout(void)
{
        cr_redirect_stdout();
        cr_redirect_stderr();
}

/*
**------------------------------------------------------------------------------
*/

TestSuite(term_is_valid);

Test(term_is_valid, basic, .init=redirect_all_stdout)
{
	char *env1;
	char *env2;

	env1 = "TERM=non_valid_term";
	env2 = "TERM=vt100";
	cr_expect_eq(term_is_valid(&env1), FUNCT_FAILURE);
	cr_expect_eq(term_is_valid(&env2), FUNCT_SUCCESS);
}

/*
**------------------------------------------------------------------------------
*/

TestSuite(term_init_struct);

Test(term_init_struct, basic)
{
	t_term	*term_p;

	term_p = term_init_struct();
	cr_assert(term_p != NULL);
	cr_expect_neq(term_p->termios_p, NULL);
	cr_expect_neq(term_p->old_termios_p, NULL);
}

/*
**------------------------------------------------------------------------------
*/

TestSuite(term_free_struct);

Test(term_free_struct, basic)
{
	t_term	*term_p;

	term_p = term_init_struct();

	/* not sure how to have this as prerequisite as test any other way */
	cr_assert(term_p != NULL, "prerequisite failed: term_init_struct");

	term_free_struct(&term_p);
	cr_expect_eq(term_p, NULL);
}

/*
**------------------------------------------------------------------------------
*/

TestSuite(get_environ_cpy);

Test(get_environ_cpy, basic)
{
	extern char **environ;
	char		**environ_cpy;
	int			index;

	environ_cpy = get_environ_cpy();
	index = 0;
	cr_assert(environ_cpy != NULL);
	while (environ_cpy[index] != NULL && environ[index] != NULL)
	{
		cr_expect_str_eq(environ_cpy[index], environ[index]);
		index++;
	}
	cr_expect_eq(environ_cpy[index], environ[index]);
}

/*
**------------------------------------------------------------------------------
*/

TestSuite(term_get_attributes);



Test(term_get_attributes, basic)
{
	t_term	*term_p;

	term_p = term_init_struct();

	/* not sure how to have this as prerequisite as test any other way */
	cr_assert(term_p != NULL, "prerequisite failed: term_init_struct");

	// cr_expect_eq(term_get_attributes(STDIN_FILENO, term_p), FUNCT_SUCCESS);
	cr_expect_eq(term_get_attributes(STDOUT_FILENO, term_p), FUNCT_SUCCESS);
	cr_expect_eq(term_get_attributes(STDERR_FILENO, term_p), FUNCT_SUCCESS);
}

Test(term_get_attributes, invalid_fd, .init=redirect_all_stdout)
{
	t_term	*term_p;

	cr_expect_eq(term_get_attributes(STDIN_FILENO, NULL), FUNCT_FAILURE);
	term_p = term_init_struct();

	/* not sure how to have this as prerequisite as test any other way */
	cr_assert(term_p != NULL, "prerequisite failed: term_init_struct");

	cr_expect_eq(term_get_attributes(10101, term_p), FUNCT_FAILURE);
}

/*
**------------------------------------------------------------------------------
*/

TestSuite(is_char_escaped);

Test(is_char_escaped, basic)
{
	cr_expect_eq(is_char_escaped("\\n", 1), FUNCT_SUCCESS);
	cr_expect_eq(is_char_escaped("\\\\n", 2), FUNCT_FAILURE);
	cr_expect_eq(is_char_escaped("abc\\n", 4), FUNCT_SUCCESS);
	cr_expect_eq(is_char_escaped("abc\\\\n", 5), FUNCT_FAILURE);
}

Test(is_char_escaped, edge_cases)
{
	cr_expect_eq(is_char_escaped("\\\"\\n", 3), FUNCT_SUCCESS);
	cr_expect_eq(is_char_escaped("\\\"\\\\n", 4), FUNCT_FAILURE);
	cr_expect_eq(is_char_escaped("", 0), FUNCT_FAILURE);
}

/*
**------------------------------------------------------------------------------
*/

/* TestSuite(update_quote_status);

Test(update_quote_status, basic)
{
	char quote;

	quote = '\0';
	cr_expect_eq(update_quote_status("easy\"line\"", 3, &quote), 0);
	cr_expect_eq(quote, '\0');
	quote = '\0';
	cr_expect_eq(update_quote_status("easy\"line\"", 4, &quote), 1);
	cr_expect_eq(quote, '"');
	quote = '\0';
	cr_expect_eq(update_quote_status("easy\"line\"", 5, &quote), 0);
	cr_expect_eq(quote, '\0');
	quote = '\0';
	cr_expect_eq(update_quote_status("easy'line'", 3, &quote), 0);
	cr_expect_eq(quote, '\0');
	quote = '\0';
	cr_expect_eq(update_quote_status("easy'line'", 4, &quote), 1);
	cr_expect_eq(quote, '\'');
	quote = '\0';
	cr_expect_eq(update_quote_status("easy'line'", 5, &quote), 0);
	cr_expect_eq(quote, '\0');
}

Test(update_quote_status, edge_cases)
{
	char quote;

	quote = '\'';
	cr_expect_eq(update_quote_status("h'arde\\'rline'", 7, &quote), 0);
	cr_expect_eq(quote, '\'');
	quote = '\0';
	cr_expect_eq(update_quote_status("\\'harderline", 1, &quote), 0);
	cr_expect_eq(quote, '\0');
	quote = '"';
	cr_expect_eq(update_quote_status("\"harder'line\"", 7, &quote), 0);
	cr_expect_eq(quote, '"');
	quote = '\0';
	cr_expect_eq(update_quote_status("\\\"harder'line", 8, &quote), 1);
	cr_expect_eq(quote, '\'');
} */

/*
**------------------------------------------------------------------------------
*/
TestSuite(builtin_echo);

Test(builtin_echo, basic, .init=redirect_all_stdout)
{
	char	**args;

	args = ft_strsplit("echo|-nEe|\\\\test\\a\\t\\v\\r\\n\\b\\f\\E", '|');
	builtin_echo(args);
	ft_freearray(&args);
	args = ft_strsplit("echo|-Eea|\n", '|');
	builtin_echo(args);
	ft_freearray(&args);
	args = ft_strsplit("echo|-nEe", '|');
	builtin_echo(args);
	ft_freearray(&args);
	args = ft_strsplit("echo|-E", '|');
	builtin_echo(args);
	cr_expect_stdout_eq_str("\\test\a\t\v\r\n\b\f\e-Eea \n\n\n");
}

/*
**------------------------------------------------------------------------------
*/

TestSuite(var_get_value);

Test(var_get_value, basic)
{
	char	*fakenv[] = {"LOL=didi", "PATH=lala", "PAT=lolo", NULL};
	cr_expect_str_eq(var_get_value("PATH", fakenv), "lala");
	cr_expect(var_get_value("NOEXIST", fakenv) == NULL);
}

/*
**------------------------------------------------------------------------------
*/

TestSuite(var_join_key_value);

Test(var_join_key_value, basic)
{
	cr_expect_str_eq(var_join_key_value("lolo", "lala"), "lolo=lala");
	cr_expect_str_eq(var_join_key_value("lolo===", "lala"), "lolo====lala");
	cr_expect_str_eq(var_join_key_value("lolo", "===lala"), "lolo====lala");
	cr_expect_str_eq(var_join_key_value("=", "="), "===");
	cr_expect_str_eq(var_join_key_value("", ""), "=");
	cr_expect_str_eq(var_join_key_value("", "="), "==");
	cr_expect_str_eq(var_join_key_value("=", ""), "==");
	cr_expect_str_eq(var_join_key_value("\t", "\t"), "\t=\t");
}

/*
**------------------------------------------------------------------------------
*/

TestSuite(var_set_value);

Test(var_set_value, basic)
{
	char	**fakenv;

	fakenv = ft_strsplit("LOL=didi|PATH=lala|PAT=lolo", '|');
	cr_assert(fakenv != NULL, "Failed to allocate test strings");
	var_set_value("PATH", "lala", fakenv);
	cr_expect(var_set_value("PATH", "changed", fakenv) == FUNCT_SUCCESS);
	cr_expect(var_set_value("LI", "changed", fakenv) == FUNCT_FAILURE);
	cr_expect_str_eq(fakenv[1], "PATH=changed");
}

/*
**------------------------------------------------------------------------------
*/

TestSuite(var_add_value);

Test(var_add_value, basic)
{
	char	**fakenv;

	fakenv = ft_strsplit("LOL=didi|PATH=lala|PAT=lolo", '|');
	cr_assert(fakenv != NULL, "Failed to allocate test strings");
	cr_expect(var_add_value("PATH", "changed", &fakenv) == FUNCT_SUCCESS);
	cr_expect_str_eq(fakenv[1], "PATH=changed");
	cr_expect(var_add_value("TEST", "success", &fakenv) == FUNCT_SUCCESS);
	cr_expect_str_eq(fakenv[3], "TEST=success");
}

/*
**------------------------------------------------------------------------------
*/

TestSuite(lexer_error, .init=redirect_all_stdout);

Test(lexer_error, one_item)
{
	t_tokenlst	*lst;

	lst = NULL;
	tokenlstaddback(&lst, START, NULL, 0);
	lexer_error(&lst, NULL);
	cr_expect(lst == NULL);
	cr_expect_stderr_eq_str("vsh: lexer: malloc error\n");
}

Test(lexer_error, long_list)
{
	t_tokenlst	*lst;

	lst = NULL;
	tokenlstaddback(&lst, START, NULL, 0);
	tokenlstaddback(&lst, WORD, ft_strdup("testword"), 0);
	tokenlstaddback(&lst, PIPE, NULL, 0);
	tokenlstaddback(&lst, PIPE, NULL, 0);
	tokenlstaddback(&lst, WORD, ft_strdup("testword"), 0);
	tokenlstaddback(&lst, PIPE, NULL, 0);
	tokenlstaddback(&lst, PIPE, NULL, 0);
	tokenlstaddback(&lst, PIPE, NULL, 0);
	tokenlstaddback(&lst, PIPE, NULL, 0);
	tokenlstaddback(&lst, IO_NUMBER, ft_strdup("235235"), 0);
	tokenlstaddback(&lst, PIPE, NULL, 0);
	tokenlstaddback(&lst, WORD, ft_strdup("testword"), 0);
	tokenlstaddback(&lst, PIPE, NULL, 0);
	tokenlstaddback(&lst, PIPE, NULL, 0);
	tokenlstaddback(&lst, PIPE, NULL, 0);
	tokenlstaddback(&lst, IO_NUMBER, ft_strdup("12351235"), 0);
	tokenlstaddback(&lst, IO_NUMBER, ft_strdup("1235135"), 0);
	tokenlstaddback(&lst, PIPE, NULL, 0);
	tokenlstaddback(&lst, PIPE, NULL, 0);
	tokenlstaddback(&lst, PIPE, NULL, 0);
	tokenlstaddback(&lst, PIPE, NULL, 0);
	tokenlstaddback(&lst, WORD, ft_strdup("testword"), 0);
	tokenlstaddback(&lst, IO_NUMBER, ft_strdup("1512351"), 0);
	tokenlstaddback(&lst, PIPE, NULL, 0);
	tokenlstaddback(&lst, PIPE, NULL, 0);
	tokenlstaddback(&lst, WORD, ft_strdup("testword"), 0);
	tokenlstaddback(&lst, PIPE, NULL, 0);
	tokenlstaddback(&lst, END, NULL, 0);
	lexer_error(&lst, NULL);
	cr_expect(lst == NULL);
	cr_expect_stderr_eq_str("vsh: lexer: malloc error\n");
}

Test(lexer_error, all_items)
{
	t_tokenlst	*lst;

	lst = NULL;
	tokenlstaddback(&lst, START, NULL, 0);
	tokenlstaddback(&lst, WORD, ft_strdup("testword"), 0);
	tokenlstaddback(&lst, IO_NUMBER, ft_strdup("testword"), 0);
	tokenlstaddback(&lst, ERROR, NULL, 0);
	tokenlstaddback(&lst, ASSIGN, ft_strdup("testword"), 0);
	tokenlstaddback(&lst, AND_IF, NULL, 0);
	tokenlstaddback(&lst, OR_IF, NULL, 0);
	tokenlstaddback(&lst, DLESS, NULL, 0);
	tokenlstaddback(&lst, DGREAT, NULL, 0);
	tokenlstaddback(&lst, SLESS, NULL, 0);
	tokenlstaddback(&lst, SGREAT, NULL, 0);
	tokenlstaddback(&lst, LESSAND, NULL, 0);
	tokenlstaddback(&lst, GREATAND, NULL, 0);
	tokenlstaddback(&lst, BG, NULL, 0);
	tokenlstaddback(&lst, PIPE, NULL, 0);
	tokenlstaddback(&lst, SEMICOL, NULL, 0);
	tokenlstaddback(&lst, NEWLINE, NULL, 0);
	tokenlstaddback(&lst, END,  NULL, 0);
	lexer_error(&lst, NULL);
	cr_expect(lst == NULL);
	cr_expect_stderr_eq_str("vsh: lexer: malloc error\n");
}

/*
**------------------------------------------------------------------------------
*/

TestSuite(tokenlstaddback, .init=redirect_all_stdout);

Test(tokenlstaddback, invalid_values)
{
	t_tokenlst	*lst;

	lst = NULL;
	tokenlstaddback(&lst, END, ft_strdup("testword"), 0);
	tokenlstaddback(&lst, WORD, ft_strdup("testword"), 0);
	tokenlstaddback(&lst, ERROR, NULL, 0);
	tokenlstaddback(&lst, ERROR, NULL, 0);
	tokenlstaddback(&lst, ASSIGN, NULL, 0);
	tokenlstaddback(&lst, AND_IF, ft_strdup("testword"), 0);
	tokenlstaddback(&lst, OR_IF, NULL, 0);
	tokenlstaddback(&lst, LESSAND, NULL, 0);
	tokenlstaddback(&lst, ERROR, NULL, 0);
	tokenlstaddback(&lst, SLESS, NULL, 0);
	tokenlstaddback(&lst, SGREAT, ft_strdup("testword"), 0);
	tokenlstaddback(&lst, LESSAND, NULL, 0);
	tokenlstaddback(&lst, GREATAND, NULL, 0);
	tokenlstaddback(&lst, BG, NULL, 0);
	tokenlstaddback(&lst, GREATAND, NULL, 0);
	tokenlstaddback(&lst, END, NULL, 0);
	tokenlstaddback(&lst, START, NULL, 0);
	tokenlstaddback(&lst, ERROR, ft_strdup("testword"), 0);
	lexer_error(&lst, NULL);
	cr_expect(lst == NULL);
	cr_expect_stderr_eq_str("vsh: lexer: malloc error\n");
}

/*
**------------------------------------------------------------------------------
*/

TestSuite(lexer);

Test(lexer, basic)
{
	t_tokenlst	*lst;
	t_tokenlst	*tmp;
	char 		*str;

	str = ft_strdup("HOME=/ ls -la || ls 2>file \"Documents\";");
	lst = NULL;
	cr_expect(lexer(&(str), &lst) == FUNCT_SUCCESS);
	cr_expect(str == NULL);
	tmp = lst;
	cr_expect(lst->type == START);
	cr_expect(lst->value == NULL);
	lst = lst->next;
	cr_expect(lst->type == ASSIGN);
	cr_expect_str_eq(lst->value, "HOME=/");
	lst = lst->next;
	cr_expect(lst->type == WORD);
	cr_expect_str_eq(lst->value, "ls");
	lst = lst->next;
	cr_expect(lst->type == WORD);
	cr_expect_str_eq(lst->value, "-la");
	lst = lst->next;
	cr_expect(lst->type == OR_IF);
	cr_expect(lst->value == NULL);
	lst = lst->next;
	cr_expect(lst->type == WORD);
	cr_expect_str_eq(lst->value, "ls");
	lst = lst->next;
	cr_expect(lst->type == IO_NUMBER);
	cr_expect_str_eq(lst->value, "2");
	lst = lst->next;
	cr_expect(lst->type == SGREAT);
	cr_expect(lst->value == NULL);
	lst = lst->next;
	cr_expect(lst->type == WORD);
	cr_expect_str_eq(lst->value, "file");
	lst = lst->next;
	cr_expect(lst->type == WORD);
	cr_expect_str_eq(lst->value, "\"Documents\"");
	lst = lst->next;
	cr_expect(lst->type == SEMICOL);
	lst = lst->next;
	cr_expect(lst->type == END);
	cr_expect(lst->value == NULL);
	tokenlstdel(&tmp);
}