/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   shell_init_input.c                                 :+:    :+:            */
/*                                                     +:+                    */
/*   By: rkuijper <rkuijper@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/09/04 10:22:44 by rkuijper       #+#    #+#                */
/*   Updated: 2019/09/04 10:37:55 by mavan-he      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"
#include <term.h>

t_vshdatacurs		*shell_init_vshdatacurs(void)
{
	t_vshdatacurs	*curs;

	curs = ft_memalloc(sizeof(t_vshdatacurs));
	if (curs == NULL)
		return (NULL);
	curs->coords = (t_point){ 1, 1 };
	curs->cur_ws_col = UNINIT;
	curs->cur_ws_row = UNINIT;
	return (curs);
}

t_vshdataline		*shell_init_vshdataline(void)
{
	t_vshdataline	*line;

	line = ft_memalloc(sizeof(t_vshdataline));
	if (line == NULL)
		return (NULL);
	line->len_max = 64;
	return (line);
}

t_vshdataprompt		*shell_init_vshdataprompt(void)
{
	t_vshdataprompt	*prompt;

	prompt = ft_memalloc(sizeof(t_vshdataprompt));
	if (prompt == NULL)
		return (NULL);
	prompt->cur_prompt_type = REGULAR_PROMPT;
	prompt->prompt_name = PROMPT_NAME;
	prompt->prompt_seperator = PROMPT_SEPERATOR;
	prompt->prompt_len = ft_strlen(prompt->prompt_name) +
		ft_strlen(prompt->prompt_seperator);
	return (prompt);
}

t_vshdatainput		*shell_init_vshdatainput(void)
{
	t_vshdatainput	*input;

	input = ft_memalloc(sizeof(t_vshdatainput));
	if (input == NULL)
		return (NULL);
	return (input);
}

t_vshdatatermcaps	*shell_init_vshdatatermcaps(void)
{
	t_vshdatatermcaps	*termcaps;

	termcaps = ft_memalloc(sizeof(t_vshdatatermcaps));
	if (termcaps == NULL)
		return (NULL);
	termcaps->tc_clear_lines_str = tgetstr("cd", NULL);
	if (termcaps->tc_clear_lines_str == NULL)
	{
		ft_memdel((void**)&termcaps);
		return (NULL);
	}
	termcaps->tc_scroll_down_str = tgetstr("sf", NULL);
	if (termcaps->tc_scroll_down_str == NULL)
	{
		ft_strdel(&termcaps->tc_clear_lines_str);
		ft_memdel((void**)&termcaps);
		return (NULL);
	}
	return (termcaps);
}
