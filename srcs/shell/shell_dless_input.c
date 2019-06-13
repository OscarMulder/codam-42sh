/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   shell_dless_input.c                                :+:    :+:            */
/*                                                     +:+                    */
/*   By: jbrinksm <jbrinksm@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/06/02 13:23:16 by jbrinksm       #+#    #+#                */
/*   Updated: 2019/06/13 16:08:03 by jbrinksm      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"

int		shell_dless_read_till_stop(char **heredoc, char *stop)
{
	char	*temp;
	int		done;

	temp = NULL;
	done = false;
	while (done == false)
	{
		input_read(&temp);
		if (temp == NULL)
			return (FUNCT_ERROR);
		done = ft_strequ(temp, stop);
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
	if (*heredoc == NULL)
		return (FUNCT_FAILURE);
	return (FUNCT_SUCCESS);
}

int		shell_dless_set_tk_val(t_tokenlst *probe, char **heredoc, char *stop)
{
	int	ret;

	ft_strdel(&(probe->value));
	ret = shell_dless_read_till_stop(heredoc, stop);
	if (ret == FUNCT_SUCCESS)
		probe->value = ft_strdup(*heredoc);
	else if (ret == FUNCT_FAILURE)
		probe->value = ft_strnew(0);
	if (probe->value == NULL)
		return (FUNCT_ERROR);
	return (FUNCT_SUCCESS);
}

/*
**	Right now this function will not include any '\n'
**	in between the lines read. This should be fine since I'm
**	pretty sure our input_read is supposed to read those '\n's
**	whenever you press return.
*/

int		shell_dless_input(t_tokenlst *token_lst)
{
	char		*heredoc;
	t_tokenlst	*probe;
	char		*stop;

	probe = token_lst;
	heredoc = NULL;
	while (probe != NULL)
	{
		if (probe->type == DLESS)
		{
			probe = probe->next;
			stop = ft_strdup(probe->value);
			if (stop == NULL || shell_dless_set_tk_val(probe, &heredoc, stop)
			== FUNCT_ERROR)
			{
				ft_printf("vsh: failed to allocate enough memory for "
				"heredoc\n");
				return (FUNCT_ERROR);
			}
			ft_strdel(&heredoc);
			ft_strdel(&stop);
		}
		probe = probe->next;
	}
	return (FUNCT_SUCCESS);
}
