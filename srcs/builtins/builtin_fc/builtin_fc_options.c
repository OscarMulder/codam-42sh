/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   builtin_fc_options.c                               :+:    :+:            */
/*                                                     +:+                    */
/*   By: omulder <omulder@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/09/11 12:38:27 by omulder        #+#    #+#                */
/*   Updated: 2019/09/21 16:24:51 by omulder       ########   odam.nl         */
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
	if (fc->first == NULL)
		return (fc_handle_first_last(i, args, fc));
	return (0);
}

void	fc_option_suppress(t_fcdata *fc)
{
	fc->options |= FC_OPT_N;
}

void	fc_option_reverse(t_fcdata *fc)
{
	fc->options |= FC_OPT_R;
}

int		fc_option_substitute(int i, char **args, t_fcdata *fc)
{
	fc->options |= FC_OPT_S;
	if (fc->first != NULL)
		return (0);
	i++;
	if (args[i] == NULL || (args[i][0] == '-' &&
	ft_isdigit(args[i][1]) == false))
		return (0);
	if (ft_strchr(args[i], '=') != NULL)
		fc->replace = args[i];
	else
	{
		fc->first = args[i];
		return (1);
	}
	i++;
	if (args[i] == NULL || (args[i][0] == '-' &&
	ft_isdigit(args[i][1]) == false))
		return (1);
	fc->first = args[i];
	return (2);
}
