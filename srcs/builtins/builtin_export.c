/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   builtin_export.c                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: mavan-he <mavan-he@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/05/03 18:16:19 by mavan-he       #+#    #+#                */
/*   Updated: 2019/05/03 19:59:20 by mavan-he      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

/*
** WORK IN PROGRESS
** export: usage: export [-n] [name[=value] ...] or export -p
**
** TO DO:
** NO ARGS:
**	List var_extern, including empty keys.
** Read in flags -n -p  (remove from var_extern to var_intern)
** OPTION -p
** List var_extern except if args are given
** OPTION -n
** remove arg keys from var_extern to var_intern (or add new key when it does not exist)
** ARGS:
**	for each arg:
	- If the key is followed by =value, the value of the key is set to value.
	- check if key exists in var_intern (if yes, remove)
	- check if key exists in var_extern (if yes, replace, return)
	- else add key to var_extern

	ERROR:
	export =hoi
bash: export: `=hoi': not a valid identifier
	export -x
bash: export: -x: invalid option
export: usage: export [-n] [name[=value] ...] or export -p

??????????? If the -f option is supplied, the names refer to shell functions; otherwise the names refer to shell variables.
*/

#include "vsh.h"

int			builtin_export(char **args, char **var_intern, char ***var_extern)
{

	return (FUNCT_SUCCESS);
}
