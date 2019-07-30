/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   shell_dless_input.c                                :+:    :+:            */
/*                                                     +:+                    */
/*   By: jbrinksm <jbrinksm@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/06/02 13:23:16 by jbrinksm       #+#    #+#                */
/*   Updated: 2019/07/30 13:22:50 by jbrinksm      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"
#include <unistd.h>

int		shell_dless_read_till_stop(char **heredoc, char *stop, t_vshdata *vshdata)
{
	char	*temp;
	int		done;
	int		status;

	temp = NULL;
	done = false;
	while (done == false)
	{
		ft_putstr("> ");
		if (input_read(vshdata, &temp, &status) == FUNCT_ERROR)
			return (FUNCT_ERROR);
		done = ft_strequ(temp, stop);
		ft_putstr("\n");
		if (done == true)
			continue ;
		if (*heredoc == NULL)
			*heredoc = ft_strdup(temp);
		else
			*heredoc = ft_strjoinfree_s1(*heredoc, temp);
		ft_strdel(&temp);
		if (*heredoc == NULL)
			return (FUNCT_ERROR);
	}
	ft_strdel(&temp);
	return (FUNCT_SUCCESS);
}

int		shell_dless_set_tk_val(t_tokenlst *probe, char **heredoc, char *stop, t_vshdata *vshdata)
{
	int	ret;

	ft_strdel(&(probe->value));
	ret = shell_dless_read_till_stop(heredoc, stop, vshdata);
	if (ret == FUNCT_SUCCESS)
	{
		if (*heredoc != NULL)
			probe->value = ft_strdup(*heredoc);
		else
			probe->value = ft_strnew(0);
	}
	if (probe->value == NULL)
		return (FUNCT_ERROR);
	return (FUNCT_SUCCESS);
}

static bool	is_valid_stoptoken(t_tokenlst *token)
{
	g_state->exit_code = EXIT_FAILURE;
	if (token->type != WORD && token->type != ASSIGN)
	{
		ft_eprintf("vsh: syntax error near unexpected token '%s'\n",
			parser_return_token_str(token->type));
		return (false);
	}
	if (token->value == NULL)
	{
		ft_eprintf("vsh: '%s' is not a valid heredoc delimiter\n",
			token->value);
		return (false);
	}
	g_state->exit_code = EXIT_SUCCESS;
	return (true);
}

int		shell_dless_input(t_vshdata *vshdata, t_tokenlst **token_lst)
{
	char		*heredoc;
	t_tokenlst	*probe;
	char		*stop;

	probe = *token_lst;
	heredoc = NULL;
	while (probe != NULL)
	{
		if (probe->type == DLESS)
		{
			probe = probe->next;
			if (is_valid_stoptoken(probe) == false)
				return (FUNCT_ERROR);
			stop = ft_strjoin(probe->value, "\n");
			if (stop == NULL || shell_dless_set_tk_val(probe, &heredoc,
			stop, vshdata) == FUNCT_ERROR)
			{
				ft_eprintf("vsh: failed to allocate memory for heredoc\n");
				return (FUNCT_ERROR);
			}
			ft_strdel(&heredoc);
			ft_strdel(&stop);
		}
		probe = probe->next;
	}
	return (FUNCT_SUCCESS);
}
