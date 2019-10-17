/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   glob_expand_word.c                                 :+:    :+:            */
/*                                                     +:+                    */
/*   By: jbrinksm <jbrinksm@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/10/07 14:54:03 by jbrinksm       #+#    #+#                */
/*   Updated: 2019/10/17 19:16:52 by mavan-he      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"

int			glob_init_path(t_globtoken **tokenlst, char **path)
{
	if ((*tokenlst)->tk_type == GLOB_SLASH || (*tokenlst)->tk_type == GLOB_DOTSLASH ||
		(*tokenlst)->tk_type == GLOB_DOTDOTSLASH)
	{
		if (glob_add_dotslash_to_path(tokenlst, path) == FUNCT_ERROR)
			return (FUNCT_ERROR); // error
	}
	else
	{
		*path = getcwd(NULL, 0);
		if (*path == NULL)
			return (FUNCT_ERROR); // error
		*path = ft_strjoinfree_s1(*path, "/");
	}
	if (*path == NULL)
		return (FUNCT_ERROR); // error
	return (FUNCT_SUCCESS);
}

int		glob_expand_word(char *word)
{
	t_globtoken		*tokenlst;
	t_globtoken		*tokenlst_head;
	char			*path;

	if (word == NULL)
		return (FUNCT_ERROR);
	path = NULL;
	tokenlst = NULL;
	if (glob_lexer(&tokenlst, word) == FUNCT_ERROR)
		return (FUNCT_ERROR);
	tokenlst_head = tokenlst;
	glob_print_tokenlist(tokenlst); // debug
	if (glob_init_path(&tokenlst, &path) == FUNCT_ERROR)
		return (FUNCT_ERROR);
	ft_printf("Begin path: %s\n", path); // debug
	if (glob_loop_matcher(tokenlst, path) == FUNCT_ERROR)
		return (FUNCT_ERROR);
	return (FUNCT_SUCCESS);
}
