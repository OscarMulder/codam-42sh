/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   auto_match_builtins.c                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: mavan-he <mavan-he@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/08/10 20:15:06 by mavan-he       #+#    #+#                */
/*   Updated: 2019/09/02 16:59:12 by mavan-he      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"

int		auto_match_builtins(char *match, t_list **matchlst, int match_len)
{
	char	*builtin;

	builtin = NULL;

	if (ft_strnequ(match, "exit", match_len))
		builtin = "exit";
	else if (ft_strnequ(match, "export", match_len))
		builtin = "export";
	else if (ft_strnequ(match, "set", match_len))
		builtin = "set";
	else if (ft_strnequ(match, "unset", match_len))
		builtin = "unset";
	else if (ft_strnequ(match, "history", match_len))
		builtin = "history";
	return (builtin == NULL ?
		FUNCT_FAILURE : auto_add_tolst(matchlst, builtin));
}
