/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   builtin_assign_test.c                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: jbrinksm <jbrinksm@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/06/07 16:55:26 by jbrinksm       #+#    #+#                */
/*   Updated: 2019/08/19 11:43:51 by mavan-he      ########   odam.nl         */
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

Test(builtin_assign, basic_change_existingvar_1)
{
	t_vshdata	data;

	g_state = (t_state*)ft_memalloc(sizeof(t_state));
	hash_init(&vshdata);
	data.envlst = env_getlst();
	cr_assert(data.envlst != NULL);
	g_state->exit_code = 0;
	builtin_assign("PATH=nothing", &vshdata, ENV_EXTERN);
	while (data.envlst != NULL && ft_strnequ(data.envlst->var, "PATH", 4) == 0)
		data.envlst = data.envlst->next;
	cr_assert(data.envlst != NULL);
	cr_expect_str_eq(ft_itoa(g_state->exit_code), ft_itoa(EXIT_SUCCESS));
	cr_expect_str_eq(ft_itoa(data.envlst->type), ft_itoa(ENV_EXTERN));
	cr_expect_str_eq(data.envlst->var, "PATH=nothing");
}

Test(builtin_assign, basic_add_newvar_1)
{
	t_vshdata	data;

	g_state = (t_state*)ft_memalloc(sizeof(t_state));

	data.envlst = env_getlst();
	cr_assert(data.envlst != NULL);
	g_state->exit_code = 0;
	builtin_assign("NeWvAr=blabla", &vshdata, ENV_LOCAL);
	while (data.envlst != NULL && ft_strnequ(data.envlst->var, "NeWvAr", 6) == 0)
		data.envlst = data.envlst->next;
	cr_assert(data.envlst != NULL);
	cr_expect_str_eq(ft_itoa(g_state->exit_code), ft_itoa(EXIT_SUCCESS));
	cr_expect_str_eq(ft_itoa(data.envlst->type), ft_itoa(ENV_LOCAL));
	cr_expect_str_eq(data.envlst->var, "NeWvAr=blabla");
}

Test(builtin_assign, basic_change_existingvar_2)
{
	t_vshdata	data;

	g_state = (t_state*)ft_memalloc(sizeof(t_state));

	data.envlst = env_getlst();
	cr_assert(data.envlst != NULL);
	g_state->exit_code = 0;
	builtin_assign("SHELL=jbrinksm", &vshdata, ENV_EXTERN);
	while (data.envlst != NULL && ft_strnequ(data.envlst->var, "SHELL", 4) == 0)
		data.envlst = data.envlst->next;
	cr_assert(data.envlst != NULL);
	cr_expect_str_eq(ft_itoa(g_state->exit_code), ft_itoa(EXIT_SUCCESS));
	cr_expect_str_eq(ft_itoa(data.envlst->type), ft_itoa(ENV_EXTERN));
	cr_expect_str_eq(data.envlst->var, "SHELL=jbrinksm");
}

Test(builtin_assign, test_failed_exitcode_1)
{
	t_vshdata	data;

	g_state = (t_state*)ft_memalloc(sizeof(t_state));

	data.envlst = env_getlst();
	cr_assert(data.envlst != NULL);
	g_state->exit_code = 0;
	builtin_assign(NULL, &vshdata, ENV_LOCAL);
	cr_assert(data.envlst != NULL);
	cr_expect_str_eq(ft_itoa(g_state->exit_code), ft_itoa(EXIT_FAILURE));
}

/*
**------------------------------------------------------------------------------
*/
