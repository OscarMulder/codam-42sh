/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   builtin_fc_init.c                                  :+:    :+:            */
/*                                                     +:+                    */
/*   By: omulder <omulder@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/09/11 12:45:01 by omulder        #+#    #+#                */
/*   Updated: 2019/09/13 14:37:26 by omulder       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"

void	fc_set_options(char **args, t_fcdata *fc)
{
	int i;

	i = 1;
	while (args[i] != NULL && ft_strequ(args[i], "--") == false)
	{
		if (ft_strequ(args[i], "-e"))
			i += fc_option_editor(i, args, fc);
		else if (ft_strequ(args[i], "-l"))
			i += fc_option_list(i, args, fc);
		else if (ft_strequ(args[i], "-n"))
			fc_option_suppress(fc);
		else if (ft_strequ(args[i], "-r"))
			fc_option_reverse(fc);
		else if (ft_strequ(args[i], "-s"))
			i += fc_option_substitute(i, args, fc);
		else if (args[i][0] == '-')
		{
			ft_eprintf(E_FC_INV_OPT, args[i]);
			ft_eprintf(U_FC);
			return ;
		}
		i++;
	}
}

void	fc_set_default_editor(t_vshdata *data, t_fcdata *fc)
{
	fc->editor = env_getvalue("FCEDIT", data->envlst);
	if (fc->editor == NULL)
		fc->editor = DEF_FCEDIT;
}
