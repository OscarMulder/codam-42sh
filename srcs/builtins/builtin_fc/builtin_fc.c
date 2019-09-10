/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   builtin_fc.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: omulder <omulder@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/09/10 12:23:10 by omulder        #+#    #+#                */
/*   Updated: 2019/09/10 18:26:59 by omulder       ########   odam.nl         */
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
** -n <-- surpres numbers
** -r <-- reverse order of commands listed or edited
** -s [old=new][first]<-- re-execute without editor
*/

void		set_option_flag(char **args, t_fcdata *fc)
{
	// ok.
}

void		set_default_editor(t_vshdata *data, t_fcdata *fc)
{
	fc->editor = env_getvalue("FCEDIT", data->envlst);
	if (fc->editor == NULL)
		fc->editor == DEF_FCEDIT;
}

void		builtin_fc(char **args, t_vshdata *data)
{
	return (FUNCT_SUCCESS);
}
