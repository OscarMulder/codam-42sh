/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   builtin_fc_init.c                                  :+:    :+:            */
/*                                                     +:+                    */
/*   By: omulder <omulder@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/09/11 12:45:01 by omulder        #+#    #+#                */
/*   Updated: 2019/09/20 19:52:44 by omulder       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"

static int	fc_arg_error(char c)
{
	ft_eprintf(E_FC_INV_OPT, c);
	ft_eprintf(U_FC);
	return (FUNCT_FAILURE);
}

int			fc_handle_option(char c, int *i, char **args, t_fcdata *fc)
{
	if (c == 'e')
		(*i) += fc_option_editor(*i, args, fc);
	else if (c == 'l')
		(*i) += fc_option_list(*i, args, fc);
	else if (c == 'n')
		fc_option_suppress(fc);
	else if (c == 'r')
		fc_option_reverse(fc);
	else if (c == 's')
		(*i) += fc_option_substitute(*i, args, fc);
	else
		return (FUNCT_FAILURE);
	return (FUNCT_SUCCESS);
}

int			fc_set_options(char **args, t_fcdata *fc)
{
	int i;
	int new_i;
	int j;

	i = 1;
	while (args[i] != NULL && ft_strequ(args[i], "--") == false)
	{
		if (args[i][0] == '-')
		{
			j = 1;
			new_i = i;
			while (args[i][j] != '\0')
			{
				if (fc_handle_option(args[i][j], &new_i, args, fc)
				== FUNCT_FAILURE)
					return (fc_arg_error(args[i][j]));
				j++;
			}
			i = new_i;
		}
		i++;
	}
	return (FUNCT_SUCCESS);
}

void		fc_set_default_editor(t_vshdata *data, t_fcdata *fc)
{
	fc->editor = env_getvalue("FCEDIT", data->envlst);
	if (fc->editor == NULL)
		fc->editor = DEF_FCEDIT;
}
