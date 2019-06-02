/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   shell_dless_input.c                                :+:    :+:            */
/*                                                     +:+                    */
/*   By: jbrinksm <jbrinksm@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/06/02 13:23:16 by jbrinksm       #+#    #+#                */
/*   Updated: 2019/06/02 13:59:38 by jbrinksm      ########   odam.nl         */
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

/*
**	Right now this function will not include any '\n'
**	in between the lines read. This should be fine since I'm
**	pretty sure our input_read is supposed to read those '\n's
**	whenever you press return.
*/

void	shell_dless_input(t_tokenlst *token_lst)
{
	char 		*heredoc;
	t_tokenlst	*probe;
	char		*stop;
	int			ret;

	probe = token_lst;
	heredoc = NULL;
	while (probe != NULL)
	{
		if (probe->type == DLESS)
		{
			probe = probe->next;
			stop = ft_strdup(probe->value);
			ft_strdel(&(probe->value));
			ret = shell_dless_read_till_stop(&heredoc, stop) == FUNCT_SUCCESS;
			if (ret == FUNCT_SUCCESS)
				probe->value = ft_strdup(heredoc);
			else if (ret == FUNCT_FAILURE)
				probe->value = ft_strnew(0);
			else
				ft_printf("NICE MALLOC ERROR THAT IDK HOW TO HANDLE"
				"WHAT DO WE DO HERE BOIS \n");
			ft_strdel(&heredoc);
			ft_strdel(&stop);
		}
		probe = probe->next;
	}
}
