/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   shell_prompt.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: jbrinksm <jbrinksm@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/04/11 20:16:38 by jbrinksm       #+#    #+#                */
/*   Updated: 2019/08/26 13:40:51 by jbrinksm      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"

void	shell_get_valid_prompt(t_vshdata *vshdata, int prompt_type)
{
	if (prompt_type == QUOTE_PROMPT)
		vshdata->prompt_name = "quote";
	else if (prompt_type == DQUOTE_PROMPT)
		vshdata->prompt_name = "dquote";
	else
		vshdata->prompt_name = "vsh ";
	vshdata->prompt_seperator = "> ";
	if (vshdata->prompt_addition != NULL)
		vshdata->prompt_len = ft_strlen(vshdata->prompt_name)
			+ ft_strlen(vshdata->prompt_seperator)
			+ ft_strlen(vshdata->prompt_addition) + 1; // 1 is for padding
	else
		vshdata->prompt_len = ft_strlen(vshdata->prompt_name)
		+ ft_strlen(vshdata->prompt_seperator);
}

void	shell_display_prompt(t_vshdata *vshdata, int prompt_type)
{
	char	*cwd;
	char	promptbuf;

	cwd = env_getvalue("PWD", vshdata->envlst);
	vshdata->prompt_addition = shell_getcurrentdir(cwd);
	ft_eprintf("[%s]\n", vshdata->prompt_addition);
	shell_get_valid_prompt(vshdata, prompt_type);
	vshdata->cur_prompt_type = prompt_type;
	if (prompt_type == REGULAR_PROMPT)
		ft_printf(RED);
	ft_printf("%s", vshdata->prompt_name);
	if (vshdata->prompt_addition != NULL)
	{
		ft_printf(BLU);
		ft_printf("%s ", vshdata->prompt_addition);
	}
	if (prompt_type == REGULAR_PROMPT && g_state->exit_code == EXIT_SUCCESS)
		ft_printf(YEL);
	else if (prompt_type == REGULAR_PROMPT)
		ft_printf(RED);
	ft_printf("%s" RESET, vshdata->prompt_seperator);
}
