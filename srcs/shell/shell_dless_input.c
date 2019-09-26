/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   shell_dless_input.c                                :+:    :+:            */
/*                                                     +:+                    */
/*   By: jbrinksm <jbrinksm@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/06/02 13:23:16 by jbrinksm       #+#    #+#                */
/*   Updated: 2019/09/26 14:25:15 by tde-jong      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"
#include <unistd.h>

static int	return_heredoc_error(void)
{
	ft_eprintf(E_N_ALLOC_STR, "heredoc");
	return (FUNCT_ERROR);
}

static int	shell_dless_read_till_stop(char **heredoc, char *heredoc_delim,
			t_vshdata *data)
{
	char	*line_tmp;
	int		ret;

	line_tmp = data->line->line;
	data->line->line = NULL;
	while (true)
	{
		shell_display_prompt(data, DLESS_PROMPT);
		ret = input_read(data);
		if (ret == FUNCT_ERROR || ret == NEW_PROMPT)
			return (ret);
		if (ret != IR_EOF)
			ft_putchar('\n');
		if (ft_strequ(data->line->line, heredoc_delim) == true || ret == IR_EOF)
			break ;
		*heredoc = ft_strjoinfree_s1(*heredoc, data->line->line);
		ft_strdel(&data->line->line);
		if (*heredoc == NULL)
			return (return_heredoc_error());
	}
	ft_strdel(&data->line->line);
	data->line->line = line_tmp;
	return (ret);
}

static int	shell_dless_set_tk_val(t_tokenlst *probe, char **heredoc,
			char *heredoc_delim, t_vshdata *data)
{
	int	ret;

	ft_strdel(&(probe->value));
	*heredoc = ft_strnew(0);
	ret = shell_dless_read_till_stop(heredoc, heredoc_delim, data);
	if (ret == FUNCT_SUCCESS || ret == IR_EOF)
		probe->value = ft_strdup(*heredoc);
	if (probe->value == NULL || ret == NEW_PROMPT || ret == FUNCT_ERROR)
	{
		if (probe->value == NULL && ret != NEW_PROMPT)
			ft_eprintf(E_ALLOC_STR, "heredoc");
		ft_strdel(heredoc);
		ft_strdel(&heredoc_delim);
		return (ret);
	}
	probe->flags |= T_FLAG_ISHEREDOC;
	if (tool_check_for_special(probe->value) == true)
		probe->flags |= T_FLAG_HASSPECIAL;
	return (FUNCT_SUCCESS);
}

static bool	is_valid_heredoc_delim(t_tokenlst *token)
{
	g_state->exit_code = EXIT_FAILURE;
	if (token->type != WORD && token->type != ASSIGN)
	{
		ft_eprintf(E_SYNTAX_P,
			parser_return_token_str(token->type));
		return (false);
	}
	if (token->value == NULL)
	{
		ft_eprintf(E_P_NOT_VAL_HERE,
			token->value);
		return (false);
	}
	if (tools_contains_quoted_chars(token->value) == true)
	{
		tools_remove_quotes_etc(token->value, false);
		token->flags |= T_FLAG_HEREDOC_NOEXP;
	}
	g_state->exit_code = EXIT_SUCCESS;
	return (true);
}

int			shell_dless_input(t_vshdata *data, t_tokenlst **token_lst)
{
	char		*heredoc;
	t_tokenlst	*probe;
	char		*heredoc_delim;
	int			ret;

	probe = *token_lst;
	while (probe != NULL)
	{
		if (probe->type == DLESS)
		{
			probe = probe->next;
			if (is_valid_heredoc_delim(probe) == false)
				return (FUNCT_ERROR);
			heredoc_delim = ft_strjoin(probe->value, "\n");
			if (heredoc_delim == NULL)
				return (return_heredoc_error());
			ret = shell_dless_set_tk_val(probe, &heredoc, heredoc_delim, data);
			if (ret == FUNCT_ERROR || ret == NEW_PROMPT)
				return (ret);
			ft_strdel(&heredoc);
			ft_strdel(&heredoc_delim);
		}
		probe = probe->next;
	}
	return (FUNCT_SUCCESS);
}
