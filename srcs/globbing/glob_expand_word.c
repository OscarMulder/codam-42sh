/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   glob_expand_word.c                                 :+:    :+:            */
/*                                                     +:+                    */
/*   By: jbrinksm <jbrinksm@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/10/07 14:54:03 by jbrinksm       #+#    #+#                */
/*   Updated: 2019/10/20 12:13:29 by mavan-he      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"

static int	glob_init_path(t_globtoken **tokenlst, char **path, int *cwd_len)
{
	if ((*tokenlst)->tk_type == GLOB_SLASH ||
		(*tokenlst)->tk_type == GLOB_DOTSLASH ||
		(*tokenlst)->tk_type == GLOB_DOTDOTSLASH)
	{
		if (glob_add_dotslash_to_path(tokenlst, path) == FUNCT_ERROR)
			return (FUNCT_ERROR);
	}
	else
	{
		*path = getcwd(NULL, 0);
		if (*path == NULL)
			return (err_ret_exit(E_ALLOC_STR, EXIT_FAILURE));
		*path = ft_strjoinfree_s1(*path, "/");
		*cwd_len = ft_strlen(*path);
	}
	if (*path == NULL)
		return (err_ret_exit(E_ALLOC_STR, EXIT_FAILURE));
	return (FUNCT_SUCCESS);
}

static void	glob_free(t_globtoken **tokenlst, char **path)
{
	glob_del_tokenlst(tokenlst);
	ft_strdel(path);
}

int		glob_expand_word(t_ast **ast, char *word)
{
	t_globtoken		*tokenlst;
	t_globtoken		*tokenlst_head;
	char			*path;
	int				cwd_len;

	cwd_len = 0;
	if (word == NULL)
		return (FUNCT_ERROR);
	path = NULL;
	tokenlst = NULL;
	if (glob_lexer(&tokenlst, word) == FUNCT_ERROR)
		return (FUNCT_ERROR);
	tokenlst_head = tokenlst;
	if (glob_init_path(&tokenlst, &path, &cwd_len) == FUNCT_ERROR)
	{
		glob_free(&tokenlst_head, &path);
		return (FUNCT_ERROR);
	}
	if (glob_loop_matcher(ast, tokenlst, path, cwd_len) == FUNCT_ERROR)
	{
		glob_free(&tokenlst_head, &path);
		return (FUNCT_ERROR);
	}
	glob_free(&tokenlst_head, &path);
	return (FUNCT_SUCCESS);
}
