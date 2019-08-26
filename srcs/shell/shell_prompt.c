/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   shell_prompt.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: jbrinksm <jbrinksm@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/04/11 20:16:38 by jbrinksm       #+#    #+#                */
/*   Updated: 2019/08/26 15:33:07 by rkuijper      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"

void	shell_get_valid_prompt(t_vshdata *data, int prompt_type)
{
	if (prompt_type == QUOTE_PROMPT)
		data->prompt_name = "quote";
	else if (prompt_type == DQUOTE_PROMPT)
		data->prompt_name = "dquote";
	else
		data->prompt_name = "vsh ";
	data->prompt_seperator = "> ";
	if (data->prompt_addition != NULL)
		data->prompt_len = ft_strlen(data->prompt_name)
			+ ft_strlen(data->prompt_seperator)
			+ ft_strlen(data->prompt_addition) + 1; // 1 is for padding
	else
		data->prompt_len = ft_strlen(data->prompt_name)
		+ ft_strlen(data->prompt_seperator);
}

void	shell_display_prompt(t_vshdata *data, int prompt_type)
{
	char	*cwd;

	cwd = env_getvalue("PWD", data->envlst);
	data->prompt_addition = shell_getcurrentdir(cwd);
	ft_eprintf("[%s]\n", data->prompt_addition);
	shell_get_valid_prompt(data, prompt_type);
	data->cur_prompt_type = prompt_type;
	if (prompt_type == REGULAR_PROMPT)
		ft_printf(RED);
	ft_printf("%s", data->prompt_name);
	if (data->prompt_addition != NULL)
	{
		ft_printf(BLU);
		ft_printf("%s ", data->prompt_addition);
	}
	if (prompt_type == REGULAR_PROMPT && g_state->exit_code == EXIT_SUCCESS)
		ft_printf(YEL);
	else if (prompt_type == REGULAR_PROMPT)
		ft_printf(RED);
	ft_printf("%s" RESET, data->prompt_seperator);
}
