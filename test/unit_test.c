/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   unit_test.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: jbrinksm <jbrinksm@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/04/18 16:37:32 by omulder        #+#    #+#                */
/*   Updated: 2019/05/29 17:30:34 by jbrinksm      ########   odam.nl         */
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
#include <limits.h>
#include <unistd.h>
#include <fcntl.h>
#include "vsh_history.h"

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

TestSuite(env_get_environ_cpy);

Test(env_get_environ_cpy, basic)
{
	extern char **environ;
	char		**environ_cpy;
	int			index;

	environ_cpy = env_get_environ_cpy();
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

TestSuite(tools_is_char_escaped);

Test(tools_is_char_escaped, basic)
{
	cr_expect_eq(tools_is_char_escaped("\\n", 1), true);
	cr_expect_eq(tools_is_char_escaped("\\\\n", 2), false);
	cr_expect_eq(tools_is_char_escaped("abc\\n", 4), true);
	cr_expect_eq(tools_is_char_escaped("abc\\\\n", 5), false);
}

Test(tools_is_char_escaped, edge_cases)
{
	cr_expect_eq(tools_is_char_escaped("\\\"\\n", 3), true);
	cr_expect_eq(tools_is_char_escaped("\\\"\\\\n", 4), false);
	cr_expect_eq(tools_is_char_escaped("", 0), false);
}

/*
**------------------------------------------------------------------------------
*/

TestSuite(shell_quote_checker_find_quote);

Test(shell_quote_checker_find_quote, basic)
{
	char *line;

	line = "A simple \"line\"";
	cr_expect_eq(shell_quote_checker_find_quote(line), '\0');
	line = "A simple \"line";
	cr_expect_eq(shell_quote_checker_find_quote(line), '"');
	line = "A simple line";
	cr_expect_eq(shell_quote_checker_find_quote(line), '\0');
	line = "A simple line\"";
	cr_expect_eq(shell_quote_checker_find_quote(line), '"');
	line = "A simple 'line'";
	cr_expect_eq(shell_quote_checker_find_quote(line), '\0');
	line = "A simple 'line";
	cr_expect_eq(shell_quote_checker_find_quote(line), '\'');
	line = "A simple line";
	cr_expect_eq(shell_quote_checker_find_quote(line), '\0');
	line = "A simple line'";
	cr_expect_eq(shell_quote_checker_find_quote(line), '\'');
}

Test(shell_quote_checker_find_quote, both_quotes)
{
	char *line;

	line = "A simple \"line\'";
	cr_expect_eq(shell_quote_checker_find_quote(line), '"');
	line = "A simple 'line\"";
	cr_expect_eq(shell_quote_checker_find_quote(line), '\'');
	line = "A simple ''line\"";
	cr_expect_eq(shell_quote_checker_find_quote(line), '"');
	line = "'A simple 'line\"";
	cr_expect_eq(shell_quote_checker_find_quote(line), '"');
	line = "A simple 'line\"\"";
	cr_expect_eq(shell_quote_checker_find_quote(line), '\'');
	line = "A simple 'line\"'\"'";
	cr_expect_eq(shell_quote_checker_find_quote(line), '"');
}

/*
**------------------------------------------------------------------------------
*/

TestSuite(shell_quote_checker);

/*	
**	Not sure how to test this properly yet, since you need input if
**	the quoting isn't correct.
*/

Test(shell_quote_checker, basic)
{
	char *line;

	line = strdup("lala");
	shell_quote_checker(&line);
	cr_expect_str_eq(line, "lala");
	ft_strdel(&line);
	line = strdup("lala''");
	shell_quote_checker(&line);
	cr_expect_str_eq(line, "lala''");
	ft_strdel(&line);
	line = strdup("lala\"\"");
	shell_quote_checker(&line);
	cr_expect_str_eq(line, "lala\"\"");
	ft_strdel(&line);
	line = strdup("lala'\"'");
	shell_quote_checker(&line);
	cr_expect_str_eq(line, "lala'\"'");
	ft_strdel(&line);
}

/*
**------------------------------------------------------------------------------
*/

TestSuite(builtin_echo);

Test(builtin_echo, basic, .init=redirect_all_stdout)
{
	char	**args;
	int		exit_code;

	args = ft_strsplit("echo|-nEe|\\\\test\\a\\t\\v\\r\\n\\b\\f\\E", '|');
	exit_code = INT_MIN;
	builtin_echo(args, &exit_code);
	cr_expect(exit_code == 0);
	ft_freearray(&args);

	args = ft_strsplit("echo|-Eea|\n", '|');
	exit_code = INT_MIN;
	builtin_echo(args, &exit_code);
	cr_expect(exit_code == 0);
	ft_freearray(&args);

	args = ft_strsplit("echo|-nEe", '|');
	exit_code = INT_MIN;
	builtin_echo(args, &exit_code);
	cr_expect(exit_code == 0);
	ft_freearray(&args);

	args = ft_strsplit("echo|-E", '|');
	exit_code = INT_MIN;
	builtin_echo(args, &exit_code);
	cr_expect(exit_code == 0);

	cr_expect_stdout_eq_str("\\test\a\t\v\r\n\b\f\e-Eea \n\n\n");
}

/*
**------------------------------------------------------------------------------
*/

TestSuite(env_var_get_value);

Test(env_var_get_value, basic)
{
	char	*fakenv[] = {"LOL=didi", "PATH=lala", "PAT=lolo", NULL};
	cr_expect_str_eq(env_var_get_value("PATH", fakenv), "lala");
	cr_expect(env_var_get_value("NOEXIST", fakenv) == NULL);
}

/*
**------------------------------------------------------------------------------
*/

TestSuite(env_var_join_key_value);

Test(env_var_join_key_value, basic)
{
	cr_expect_str_eq(env_var_join_key_value("lolo", "lala"), "lolo=lala");
	cr_expect_str_eq(env_var_join_key_value("lolo===", "lala"), "lolo====lala");
	cr_expect_str_eq(env_var_join_key_value("lolo", "===lala"), "lolo====lala");
	cr_expect_str_eq(env_var_join_key_value("=", "="), "===");
	cr_expect_str_eq(env_var_join_key_value("", ""), "=");
	cr_expect_str_eq(env_var_join_key_value("", "="), "==");
	cr_expect_str_eq(env_var_join_key_value("=", ""), "==");
	cr_expect_str_eq(env_var_join_key_value("\t", "\t"), "\t=\t");
}

/*
**------------------------------------------------------------------------------
*/

TestSuite(env_var_set_value);

Test(env_var_set_value, basic)
{
	char	**fakenv;

	fakenv = ft_strsplit("LOL=didi|PATH=lala|PAT=lolo", '|');
	cr_assert(fakenv != NULL, "Failed to allocate test strings");
	env_var_set_value("PATH", "lala", fakenv);
	cr_expect(env_var_set_value("PATH", "changed", fakenv) == FUNCT_SUCCESS);
	cr_expect(env_var_set_value("LI", "changed", fakenv) == FUNCT_FAILURE);
	cr_expect_str_eq(fakenv[1], "PATH=changed");
}

/*
**------------------------------------------------------------------------------
*/

TestSuite(env_var_add_value);

Test(env_var_add_value, basic)
{
	char	**fakenv;

	fakenv = ft_strsplit("LOL=didi|PATH=lala|PAT=lolo", '|');
	cr_assert(fakenv != NULL, "Failed to allocate test strings");
	cr_expect(env_var_add_value("PATH", "changed", &fakenv) == FUNCT_SUCCESS);
	cr_expect_str_eq(fakenv[1], "PATH=changed");
	cr_expect(env_var_add_value("TEST", "success", &fakenv) == FUNCT_SUCCESS);
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
	lexer_tokenlstaddback(&lst, START, NULL, 0);
	lexer_error(&lst, NULL);
	cr_expect(lst == NULL);
	cr_expect_stderr_eq_str("vsh: lexer: malloc error\n");
}

Test(lexer_error, long_list)
{
	t_tokenlst	*lst;

	lst = NULL;
	lexer_tokenlstaddback(&lst, START, NULL, 0);
	lexer_tokenlstaddback(&lst, WORD, ft_strdup("testword"), 0);
	lexer_tokenlstaddback(&lst, PIPE, NULL, 0);
	lexer_tokenlstaddback(&lst, PIPE, NULL, 0);
	lexer_tokenlstaddback(&lst, WORD, ft_strdup("testword"), 0);
	lexer_tokenlstaddback(&lst, PIPE, NULL, 0);
	lexer_tokenlstaddback(&lst, PIPE, NULL, 0);
	lexer_tokenlstaddback(&lst, PIPE, NULL, 0);
	lexer_tokenlstaddback(&lst, PIPE, NULL, 0);
	lexer_tokenlstaddback(&lst, IO_NUMBER, ft_strdup("235235"), 0);
	lexer_tokenlstaddback(&lst, PIPE, NULL, 0);
	lexer_tokenlstaddback(&lst, WORD, ft_strdup("testword"), 0);
	lexer_tokenlstaddback(&lst, PIPE, NULL, 0);
	lexer_tokenlstaddback(&lst, PIPE, NULL, 0);
	lexer_tokenlstaddback(&lst, PIPE, NULL, 0);
	lexer_tokenlstaddback(&lst, IO_NUMBER, ft_strdup("12351235"), 0);
	lexer_tokenlstaddback(&lst, IO_NUMBER, ft_strdup("1235135"), 0);
	lexer_tokenlstaddback(&lst, PIPE, NULL, 0);
	lexer_tokenlstaddback(&lst, PIPE, NULL, 0);
	lexer_tokenlstaddback(&lst, PIPE, NULL, 0);
	lexer_tokenlstaddback(&lst, PIPE, NULL, 0);
	lexer_tokenlstaddback(&lst, WORD, ft_strdup("testword"), 0);
	lexer_tokenlstaddback(&lst, IO_NUMBER, ft_strdup("1512351"), 0);
	lexer_tokenlstaddback(&lst, PIPE, NULL, 0);
	lexer_tokenlstaddback(&lst, PIPE, NULL, 0);
	lexer_tokenlstaddback(&lst, WORD, ft_strdup("testword"), 0);
	lexer_tokenlstaddback(&lst, PIPE, NULL, 0);
	lexer_tokenlstaddback(&lst, END, NULL, 0);
	lexer_error(&lst, NULL);
	cr_expect(lst == NULL);
	cr_expect_stderr_eq_str("vsh: lexer: malloc error\n");
}

Test(lexer_error, all_items)
{
	t_tokenlst	*lst;

	lst = NULL;
	lexer_tokenlstaddback(&lst, START, NULL, 0);
	lexer_tokenlstaddback(&lst, WORD, ft_strdup("testword"), 0);
	lexer_tokenlstaddback(&lst, IO_NUMBER, ft_strdup("testword"), 0);
	lexer_tokenlstaddback(&lst, ERROR, NULL, 0);
	lexer_tokenlstaddback(&lst, ASSIGN, ft_strdup("testword"), 0);
	lexer_tokenlstaddback(&lst, AND_IF, NULL, 0);
	lexer_tokenlstaddback(&lst, OR_IF, NULL, 0);
	lexer_tokenlstaddback(&lst, DLESS, NULL, 0);
	lexer_tokenlstaddback(&lst, DGREAT, NULL, 0);
	lexer_tokenlstaddback(&lst, SLESS, NULL, 0);
	lexer_tokenlstaddback(&lst, SGREAT, NULL, 0);
	lexer_tokenlstaddback(&lst, LESSAND, NULL, 0);
	lexer_tokenlstaddback(&lst, GREATAND, NULL, 0);
	lexer_tokenlstaddback(&lst, BG, NULL, 0);
	lexer_tokenlstaddback(&lst, PIPE, NULL, 0);
	lexer_tokenlstaddback(&lst, SEMICOL, NULL, 0);
	lexer_tokenlstaddback(&lst, NEWLINE, NULL, 0);
	lexer_tokenlstaddback(&lst, END,  NULL, 0);
	lexer_error(&lst, NULL);
	cr_expect(lst == NULL);
	cr_expect_stderr_eq_str("vsh: lexer: malloc error\n");
}

/*
**------------------------------------------------------------------------------
*/

TestSuite(lexer_tokenlstaddback, .init=redirect_all_stdout);

Test(lexer_tokenlstaddback, invalid_values)
{
	t_tokenlst	*lst;

	lst = NULL;
	lexer_tokenlstaddback(&lst, END, ft_strdup("testword"), 0);
	lexer_tokenlstaddback(&lst, WORD, ft_strdup("testword"), 0);
	lexer_tokenlstaddback(&lst, ERROR, NULL, 0);
	lexer_tokenlstaddback(&lst, ERROR, NULL, 0);
	lexer_tokenlstaddback(&lst, ASSIGN, NULL, 0);
	lexer_tokenlstaddback(&lst, AND_IF, ft_strdup("testword"), 0);
	lexer_tokenlstaddback(&lst, OR_IF, NULL, 0);
	lexer_tokenlstaddback(&lst, LESSAND, NULL, 0);
	lexer_tokenlstaddback(&lst, ERROR, NULL, 0);
	lexer_tokenlstaddback(&lst, SLESS, NULL, 0);
	lexer_tokenlstaddback(&lst, SGREAT, ft_strdup("testword"), 0);
	lexer_tokenlstaddback(&lst, LESSAND, NULL, 0);
	lexer_tokenlstaddback(&lst, GREATAND, NULL, 0);
	lexer_tokenlstaddback(&lst, BG, NULL, 0);
	lexer_tokenlstaddback(&lst, GREATAND, NULL, 0);
	lexer_tokenlstaddback(&lst, END, NULL, 0);
	lexer_tokenlstaddback(&lst, START, NULL, 0);
	lexer_tokenlstaddback(&lst, ERROR, ft_strdup("testword"), 0);
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
	cr_assert(str != NULL);
	lst = NULL;
	cr_expect(lexer(&str, &lst) == FUNCT_SUCCESS);
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
	lexer_tokenlstdel(&tmp);
}

TestSuite(parser);

Test(parser, basic)
{
	t_tokenlst	*lst;
	t_ast		*ast;
	t_ast		*tmp_ast;
	char 		*str;

	str = ft_strdup("HOME=/ ls -la || ls 2>file \"Documents\";");
	lst = NULL;
	ast = NULL;
	cr_expect(lexer(&str, &lst) == FUNCT_SUCCESS);
	cr_expect(parser_start(&lst, &ast) == FUNCT_SUCCESS);
	tmp_ast = ast;
	cr_expect(ast->type == SEMICOL);
	ast = ast->child;
	cr_expect(ast->type == OR_IF);
	parser_astdel(&tmp_ast);
	cr_expect(tmp_ast == NULL);
}

/*
**------------------------------------------------------------------------------
*/

TestSuite(command_exit);

Test(command_exec, basic, .init=redirect_all_stdout)
{
	t_tokenlst	*lst;
	t_ast		*ast;
	char 		*str;
	int			exit_code;

	str = ft_strdup("1=1");
	lst = NULL;
	ast = NULL;
	cr_expect(lexer(&(str), &lst) == FUNCT_SUCCESS);
	cr_expect(parser_start(&lst, &ast) == FUNCT_SUCCESS);
	cr_expect(exec_start(ast, &exit_code) == FUNCT_FAILURE); // this fails in the first version, shoudln't fail later
	parser_astdel(&ast);
}

/*
**------------------------------------------------------------------------------
*/
/* TestSuite(history_check);

Test(history_check, history_to_file)
{
	int		fd;
	char	*array[4];
	char	buf[22];
	int 	ret;

	ft_bzero(buf, 22);
	array[0] = "check1";
	array[1] = "check2";
	array[2] = "check3";
	array[3] = NULL;
	history = array;
	cr_expect(history_to_file() == FUNCT_SUCCESS);
	fd = open("/tmp/.vsh_history", O_RDONLY);
	cr_expect(fd > 0);
	ret = read(fd, buf, 22);
	cr_expect(ret == 21);
	cr_expect(ft_strcmp(buf, "check1\ncheck2\ncheck3\n") == 0);
}

Test(history_check, get_file_content)
{
	char	*array[4];

	array[0] = ft_strdup("check1");
	array[1] = ft_strdup("check2");
	array[2] = ft_strdup("check3");
	array[3] = NULL;
	history = array;
	cr_expect(history_to_file() == FUNCT_SUCCESS);
	cr_expect(history_get_file_content() == FUNCT_SUCCESS);
	cr_expect_str_eq("check1", history[0]);
	cr_expect_str_eq("check2", history[1]);
	cr_expect_str_eq("check3", history[2]);
	cr_expect(history[3] == NULL);
} 

TestSuite(history_output);

Test(history_check, history_print, .init=redirect_all_stdout)
{
	char	*array[4];

	history = array;
	array[0] = ft_strdup("check1");
	array[1] = ft_strdup("check2");
	array[2] = ft_strdup("check3");
	array[3] = NULL;
	cr_expect(history_to_file() == FUNCT_SUCCESS);
	cr_expect(history_get_file_content() == FUNCT_SUCCESS);
	history_print();
	cr_expect_stdout_eq_str("    0  check1\n    1  check2\n    2  check3\n");
} */
/*
**------------------------------------------------------------------------------
*/

TestSuite(exec_echo);

Test(exec_echo, basic, .init=redirect_all_stdout)
{
	t_tokenlst	*lst;
	t_ast		*ast;
	char 		*str;
	int			exit_code;

	str = ft_strdup("echo hoi");
	lst = NULL;
	ast = NULL;
	cr_expect(lexer(&(str), &lst) == FUNCT_SUCCESS);
	cr_expect(parser_start(&lst, &ast) == FUNCT_SUCCESS);
	cr_expect(exec_start(ast, &exit_code) == FUNCT_SUCCESS);
	cr_expect_stdout_eq_str("hoi\n");
	parser_astdel(&ast);
}

Test(exec_echo, basic2, .init=redirect_all_stdout)
{
	t_tokenlst	*lst;
	t_ast		*ast;
	char 		*str;
	int			exit_code;

	str = ft_strdup("echo \"Hi, this is a string\"");
	lst = NULL;
	ast = NULL;
	cr_expect(lexer(&(str), &lst) == FUNCT_SUCCESS);
	cr_expect(parser_start(&lst, &ast) == FUNCT_SUCCESS);
	cr_expect(exec_start(ast, &exit_code) == FUNCT_SUCCESS);
	cr_expect_stdout_eq_str("\"Hi, this is a string\"\n");
	parser_astdel(&ast);
} 

/*
**------------------------------------------------------------------------------
*/

TestSuite(exec_cmd);

Test(exec_cmd, basic, .init=redirect_all_stdout)
{
	t_tokenlst	*lst;
	t_ast		*ast;
	char 		*str;
	char 		*cwd;
	int			exit_code;

	str = ft_strdup("/bin/pwd");
	cwd = getcwd(NULL, 0);
	lst = NULL;
	ast = NULL;
	cr_expect(lexer(&(str), &lst) == FUNCT_SUCCESS);
	cr_expect(parser_start(&lst, &ast) == FUNCT_SUCCESS);
	cr_expect(exec_start(ast, &exit_code) == FUNCT_SUCCESS);
	ft_strdel(&str);
	str = ft_strjoin(cwd, "\n");
	cr_expect_stdout_eq_str(str);
	ft_strdel(&str);
	ft_strdel(&cwd);
	parser_astdel(&ast);
}

Test(exec_cmd, basic2, .init=redirect_all_stdout)
{
	t_tokenlst	*lst;
	t_ast		*ast;
	char 		*str;
	int			exit_code;

	str = ft_strdup("/bin/echo hoi");
	lst = NULL;
	ast = NULL;
	cr_expect(lexer(&(str), &lst) == FUNCT_SUCCESS);
	cr_expect(parser_start(&lst, &ast) == FUNCT_SUCCESS);
	cr_expect(exec_start(ast, &exit_code) == FUNCT_SUCCESS);
	cr_expect_stdout_eq_str("hoi\n");
	parser_astdel(&ast);
} 

/*
**------------------------------------------------------------------------------
*/

TestSuite(exec_find_bin);

Test(exec_find_bin, basic)
{
	char 		*str;
	char		*bin;
	char		**env;

	env = env_get_environ_cpy();
	env_var_add_value("PATH", "./", &env);
	str = ft_strdup("vsh");
	bin = exec_find_binary(str, env);
	cr_expect_str_eq(bin, ".//vsh");
	ft_freearray(&env);
	ft_strdel(&bin);
	ft_strdel(&str);
}

Test(exec_find_bin, basic2)
{
	char 		*str;
	char		*bin;
	char		**env;

	env = env_get_environ_cpy();
	env_var_add_value("PATH", "/usr/bin:/bin:./", &env);
	str = ft_strdup("ls");
	bin = exec_find_binary(str, env);
	cr_expect_str_eq(bin, "/bin/ls");
	ft_freearray(&env);
	ft_strdel(&bin);
	ft_strdel(&str);
}

Test(exec_find_bin, advanced)
{
	char 		*str;
	char		*bin;
	char		**env;

	env = env_get_environ_cpy();
	ft_printf("PATH: %s\n", env_var_get_value("PATH", env));
	env_var_add_value("PATH", "/Users/travis/.rvm/gems/ruby-2.4.2/bin:/Users/travis/.rvm/gems/ruby-2.4.2@global/bin:/Users/travis/.rvm/rubies/ruby-2.4.2/bin:/Users/travis/.rvm/bin:/Users/travis/bin:/Users/travis/.local/bin:/Users/travis/.nvm/versions/node/v6.11.4/bin:/usr/local/bin:/usr/bin:/bin:/usr/sbin:/sbin:/opt/X11/bin", &env);
	str = ft_strdup("ls");
	bin = exec_find_binary(str, env);
	cr_expect_str_eq(bin, "/bin/ls");
	ft_freearray(&env);
	ft_strdel(&bin);
	ft_strdel(&str);
}
