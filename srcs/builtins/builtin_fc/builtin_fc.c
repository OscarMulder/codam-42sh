/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   builtin_fc.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: omulder <omulder@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/09/10 12:23:10 by omulder        #+#    #+#                */
/*   Updated: 2019/09/11 12:14:43 by omulder       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"

/*
** fc [-r][-e editor] [first[last]]
** fc -l[-nr] [first[last]]
** fc -s[old=new][first]
** FLAGS:
** -e [editor] <-- Choose editor, otherwise use FCEDIT or ed
** -l [first[last]] <-- list history
** -n <-- suppress numbers
** -r <-- reverse order of commands listed or edited
** -s [old=new][first]<-- re-execute without editor
*/

int		option_editor(int i, char **args, t_fcdata *fc)
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

int		option_list(int i, char **args, t_fcdata *fc)
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

void	option_suppress(t_fcdata *fc)
{
	fc->options |= FC_OPT_S;
}

void	option_reverse(t_fcdata *fc)
{
	fc->options |= FC_OPT_R;
}

int		option_substitute(int i, char **args, t_fcdata *fc)
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

void	set_options(char **args, t_fcdata *fc)
{
	int i;

	i = 1;
	while (args[i] != NULL && ft_strequ(args[i], "--") == false)
	{
		if (ft_strequ(args[i], "-e"))
			i += option_editor(i, args, fc);
		else if (ft_strequ(args[i], "-l"))
			i += option_list(i, args, fc);
		else if (ft_strequ(args[i], "-n"))
			option_suppress(fc);
		else if (ft_strequ(args[i], "-r"))
			option_reverse(fc);
		else if (ft_strequ(args[i], "-s"))
			i += option_substitute(i, args, fc);
		else if (args[i][0] == '-')
		{
			ft_eprintf(E_FC_INV_OPT, args[i]);
			ft_eprintf(U_FC);
			return ;
		}
		i++;
	}
}

void	set_default_editor(t_vshdata *data, t_fcdata *fc)
{
	fc->editor = env_getvalue("FCEDIT", data->envlst);
	if (fc->editor == NULL)
		fc->editor = DEF_FCEDIT;
}

void	init_fcdata(t_fcdata **fc)
{
	*fc = ft_memalloc(sizeof(t_fcdata));
	(*fc)->editor = NULL;
	(*fc)->first = NULL;
	(*fc)->last = NULL;
	(*fc)->match = NULL;
	(*fc)->replace = NULL;
	(*fc)->options = 0;
}

void	builtin_fc(char **args, t_vshdata *data)
{
	t_fcdata	*fc;

	(void)data;
	init_fcdata(&fc);
	set_options(args, fc);
}
