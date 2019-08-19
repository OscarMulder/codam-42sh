/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   shell_init_vshdata.c                               :+:    :+:            */
/*                                                     +:+                    */
/*   By: mavan-he <mavan-he@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/07/29 12:42:44 by mavan-he       #+#    #+#                */
/*   Updated: 2019/08/19 14:09:55 by jbrinksm      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"

int		shell_init_vshdata(t_vshdata *vshdata)
{
	vshdata->line = NULL;
	vshdata->aliaslst = NULL;
	vshdata->prompt_name = "vsh ";
	vshdata->prompt_seperator = "> ";
	vshdata->prompt_addition = NULL;
	vshdata->cur_prompt_type = REGULAR_PROMPT;
	vshdata->envlst = env_getlst();
	if (vshdata->envlst == NULL)
		return (err_ret(E_ALLOC_STR));
	if (shell_init_files(vshdata) == FUNCT_ERROR ||
		history_get_file_content(vshdata) == FUNCT_ERROR ||
		alias_read_file(vshdata) == FUNCT_ERROR)
		return (FUNCT_ERROR);
	return (FUNCT_SUCCESS);
}
