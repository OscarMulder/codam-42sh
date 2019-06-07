/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   builtin_assign_test.c                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: jbrinksm <jbrinksm@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/06/07 16:55:26 by jbrinksm       #+#    #+#                */
/*   Updated: 2019/06/07 17:22:09 by codam         ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"
#include <criterion/criterion.h>
#include <criterion/redirect.h>

void redirect_all_stdout(void);

/*
**------------------------------------------------------------------------------
*/

TestSuite(builtin_assign);

Test(builtin_assign, basic_exitcode)
{
	t_envlst    *lst;
	int			exit_code;

	lst = env_getlst();
	exit_code = 0;
	cr_assert(lst != NULL);
	builtin_assign("PATH=nothing", lst, &exit_code);
	cr_expect_str_eq(ft_itoa(exit_code), ft_itoa(EXIT_SUCCESS));
}

Test(builtin_assign, basic_findvar)
{
	t_envlst    *lst;
	int			exit_code;
	char		*newvalue;

	lst = env_getlst();
	exit_code = 0;
	cr_assert(lst != NULL);
	builtin_assign("PATH=nothing", lst, &exit_code);
	newvalue = env_getvalue("PATH", lst);
	cr_expect_str_eq(newvalue, "nothing");
}

/*
**------------------------------------------------------------------------------
*/
