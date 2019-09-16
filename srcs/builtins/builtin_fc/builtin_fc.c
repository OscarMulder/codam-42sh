/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   builtin_fc.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: omulder <omulder@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/09/10 12:23:10 by omulder        #+#    #+#                */
/*   Updated: 2019/09/13 14:37:14 by omulder       ########   odam.nl         */
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

void	builtin_fc(char **args, t_vshdata *data)
{
	t_fcdata	*fc;

	fc = (t_fcdata*)ft_memalloc(sizeof(t_fcdata));
	fc_set_default_editor(data, fc);
	fc_set_options(args, fc);
	if (fc->options & FC_OPT_L)
		fc_list(data->history, fc);
}