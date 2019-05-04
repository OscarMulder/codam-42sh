/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   unit_test.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: jbrinksm <jbrinksm@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/04/18 16:37:32 by omulder        #+#    #+#                */
/*   Updated: 2019/05/04 16:37:53 by jbrinksm      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

/*
**	General note: We have to make sure that we redirect the STDOUT to some sort
**	of file, so that we don't destroy the test_suite output.
**	(Also, see the comments at builtin_echo.)
*/

#include "vsh.h"
#include <criterion/criterion.h>

TestSuite(term_is_valid);

Test(term_is_valid, basic)
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

	cr_expect_eq(term_get_attributes(STDIN_FILENO, NULL), FUNCT_FAILURE);
	term_p = term_init_struct();

	/* not sure how to have this as prerequisite as test any other way */
	cr_assert(term_p != NULL, "prerequisite failed: term_init_struct");

	// cr_expect_eq(term_get_attributes(STDIN_FILENO, term_p), FUNCT_SUCCESS);
	cr_expect_eq(term_get_attributes(STDOUT_FILENO, term_p), FUNCT_SUCCESS);
	cr_expect_eq(term_get_attributes(STDERR_FILENO, term_p), FUNCT_SUCCESS);
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

TestSuite(update_quote_status);

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
}

/*
**------------------------------------------------------------------------------
*/

TestSuite(builtin_echo);

Test(builtin_echo, basic)
{
	/* Will need to use functions to change stdout to a tmp file from which
	we can strcmp the output and clear and reset afterwards */

	// builtin_echo({"echo", "-nEa", "\n"});
	// builtin_echo({"echo", "-nE", "\\n"});
	// builtin_echo({"echo", "-nEe", "\\\\abc\\t\\v\\r\\f\\n"});
	// builtin_echo({"echo", "-nEe"});
	// builtin_echo({"echo", "-E"});
	// builtin_echo({"echo"});
	cr_log_warn("Please read comments at builtin_echo testsuite (basic)");
}

Test(builtin_echo, return_values)
{
	/* Please add proper return values for echo (invalid flags/arguments etc) */

	// builtin_echo({"echo", "-nEaZ", "\n"});
	cr_log_warn("Please read comments at builtin_echo testsuite (return_values)");
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
	char	*fakenv[4];
	fakenv[0] = ft_strdup("LOL=didi");
	fakenv[1] = ft_strdup("PATH=lala");
	fakenv[2] = ft_strdup("PAT=lolo");
	fakenv[3] = NULL;
	cr_assert(fakenv[0] != NULL && fakenv[1] != NULL && fakenv[2] != NULL, "Failed to allocate test strings");
	var_set_value("PATH", "lala", fakenv);
	cr_expect(var_set_value("PATH", "changed", fakenv) == FUNCT_SUCCESS);
	cr_expect(var_set_value("LI", "changed", fakenv) == FUNCT_FAILURE);
	cr_expect_str_eq(fakenv[1], "PATH=changed");
}

// return (test_ret_fail("test_prompt failed!"));
// return (test_ret_fail("test_free_and_return_null failed!"));
// return (test_ret_fail("test_get_environ_cpy failed!"));
// return (test_ret_fail("test_param_to_env failed!"));
// return (test_ret_fail("test_term_is_valid failed!"));
// return (test_ret_fail("test_term_init_struct failed!"));
// return (test_ret_fail("test_term_free_struct failed!"));
// return (test_ret_fail("test_term_get_attributes failed!"));
// return (test_ret_fail("test_parser_split_commands failed!"));
// return (test_ret_fail("test_parser_strdup_command_from_line failed!"));
// return (test_ret_fail("test_parser_command_len_from_line failed!"));
// return (test_ret_fail("test_parser_total_commands_from_line failed!"));
// return (test_ret_fail("test_is_char_escaped failed!"));
// return (test_ret_fail("test_is_char_escaped failed!"));
// return (test_ret_fail("test_echo failed!"));
