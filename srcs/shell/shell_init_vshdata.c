/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   shell_init_vshdata.c                               :+:    :+:            */
/*                                                     +:+                    */
/*   By: mavan-he <mavan-he@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/07/29 12:42:44 by mavan-he       #+#    #+#                */
/*   Updated: 2019/08/26 16:55:07 by omulder       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"

t_vshdatacurs	*shell_init_vshdatacurs(void)
{
	t_vshdatacurs	*curs;

	curs = ft_memalloc(sizeof(t_vshdatacurs));
	if (curs == NULL)
		return (NULL);
	curs->coords = (t_point){ 1, 1 };
	curs->cur_ws_col = UNINIT;
	return (curs);
}

t_vshdataline	*shell_init_vshdataline(void)
{
	t_vshdataline	*line;

	line = ft_memalloc(sizeof(t_vshdataline));
	if (line == NULL)
		return (NULL);
	line->len_max = 64;
	return (line);
}

t_vshdataprompt	*shell_init_vshdataprompt(void)
{
	t_vshdataprompt	*prompt;

	prompt = ft_memalloc(sizeof(t_vshdataprompt));
	if (prompt == NULL)
		return (NULL);
	prompt->cur_prompt_type = REGULAR_PROMPT;
	prompt->prompt_name = PROMPT_NAME " ";
	prompt->prompt_seperator = PROMPT_SEPERATOR " ";
	prompt->prompt_len = ft_strlen(prompt->prompt_name) + 
		ft_strlen(prompt->prompt_seperator);
	return (prompt);
}

t_vshdatainput	*shell_init_vshdatainput(void)
{
	t_vshdatainput	*input;

	input = ft_memalloc(sizeof(t_vshdatainput));
	if (input == NULL)
		return (NULL);
	return (input);
}

t_vshdatahashtable	*shell_init_vshdatahashtable(void)
{
	t_vshdatahashtable	*hashtable;

	hashtable = ft_memalloc(sizeof(t_vshdatahashtable));
	if (hashtable == NULL)
		return (NULL);
	return (hashtable);
}

t_vshdatahistory	*shell_init_vshdatahistory(void)
{
	t_vshdatahistory	*history;

	history = ft_memalloc(sizeof(t_vshdatahistory));
	if (history == NULL)
		return (NULL);
	history->hist_index = find_start(history->history);
	history->hist_start = history->hist_index - 1;
	history->hist_first = true;
	history->history = history->history;
	return (history);
}

int		shell_init_vshdata(t_vshdata *data)
{
	ft_bzero(data, sizeof(t_vshdata));
	data->envlst = env_getlst();
	if (data->envlst == NULL)
		return (err_ret(E_ALLOC_STR));
	data->term = term_prepare(data->envlst);
	data->curs = shell_init_vshdatacurs();
	data->history = shell_init_vshdatahistory();
	data->line = shell_init_vshdataline();
	data->prompt = shell_init_vshdataprompt();
	data->input = shell_init_vshdatainput();
	data->hashtable = shell_init_vshdatahashtable();
	if (data->term == NULL || data->curs == NULL
		|| data->history == NULL || data->line == NULL
		|| data->prompt == NULL || data->input == NULL
		|| data->hashtable == NULL || data->alias == NULL)
		return (err_ret(E_ALLOC_STR));
	if (shell_init_files(data) == FUNCT_ERROR
		|| history_get_file_content(data) == FUNCT_ERROR
		|| alias_read_file(data) == FUNCT_ERROR)
		return (FUNCT_ERROR);
	return (FUNCT_SUCCESS);
}
