/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   builtin_fc_options.c                               :+:    :+:            */
/*                                                     +:+                    */
/*   By: omulder <omulder@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/09/11 12:38:27 by omulder        #+#    #+#                */
/*   Updated: 2019/09/11 12:50:28 by omulder       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"

int		fc_option_editor(int i, char **args, t_fcdata *fc)
{
	fc->options |= FC_OPT_E;
	i++;
	if (args[i] == NULL)
	{
		ft_eprintf(E_FC_REQARG, "-e");
		ft_eprintf(U_FC);
		return (0);
	}
	fc->editor = args[i];
	return (1);
}

int		fc_option_list(int i, char **args, t_fcdata *fc)
{
	fc->options |= FC_OPT_L;
	i++;
	if (args[i] == NULL || args[i][0] == '-')
		return (0);
	fc->first = args[i];
	i++;
	if (args[i] == NULL || args[i][0] == '-')
		return (1);
	fc->last = args[i];
	return (2);
}

void	fc_option_suppress(t_fcdata *fc)
{
	fc->options |= FC_OPT_S;
}

void	fc_option_reverse(t_fcdata *fc)
{
	fc->options |= FC_OPT_R;
}

int		fc_option_substitute(int i, char **args, t_fcdata *fc)
{
	fc->options |= FC_OPT_S;
	i++;
	if (args[i] == NULL || args[i][0] == '-')
		return (0);
	if (ft_strchr(args[i], '=') != NULL)
		fc->replace = args[i];
	else
	{
		fc->first = args[i];
		return (1);
	}
	i++;
	if (args[i] == NULL || args[i][0] == '-')
		return (1);
	fc->first = args[i];
	return (2);
}