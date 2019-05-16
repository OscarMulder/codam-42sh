/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   lexer_checks.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: mavan-he <mavan-he@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/05/16 17:45:31 by mavan-he       #+#    #+#                */
/*   Updated: 2019/05/16 19:07:53 by mavan-he      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"

int		lexer_double_char_tks(char *line, int *i, t_token *token)
{
	if (line[*i] == '&' && line[(*i) + 1] == '&')
		token->type = AND_IF;
	else if (line[*i] == '|' && line[(*i) + 1] == '|')
		token->type = OR_IF;
	else if (line[*i] == '<' && line[(*i) + 1] == '<')
		token->type = DLESS;
	else if (line[*i] == '>' && line[(*i) + 1] == '>')
		token->type = DGREAT;
	else if (line[*i] == '<' && line[(*i) + 1] == '&')
		token->type = LESSAND;
	else if (line[*i] == '>' && line[(*i) + 1] == '&')
		token->type = GREATAND;
	else
		return (FUNCT_FAILURE);
	*i += 2;
	return (FUNCT_SUCCESS);
}

int		lexer_single_char_tks(char *line, int *i, t_token *token)
{
	if (line[*i] == '&')
		token->type = BG;
	else if (line[*i] == '|')
		token->type = PIPE;
	else if (line[*i] == '<')
		token->type = SLESS;
	else if (line[*i] == '>')
		token->type = SGREAT;
	else if (line[*i] == ';')
		token->type = SEMICOLON;
	else
		return (FUNCT_FAILURE);
	*i += 1;
	return (FUNCT_SUCCESS);
}

int		lexer_io_tk(char *line, int *i, t_token *token)
{
	int		i_tmp;

	i_tmp = *i;
	if (ft_isdigit(line[i_tmp]) == false)
		return (FUNCT_FAILURE);
	while (ft_isdigit(line[i_tmp]) == true)
		i_tmp++;
	if (line[i_tmp] == '>' || line[i_tmp] == '<')
		token->type = IO_NUM;
	else
		return (FUNCT_FAILURE);
	token->value.io = ft_atoi(&line[*i]);
	*i += i_tmp;
	return (FUNCT_SUCCESS);
}

int		lexer_word_tk(char *line, int *i, t_token *token)
{
	int		i_tmp;

	i_tmp = *i;
	while (line[i_tmp] != 0 && (is_tk_char(line[i_tmp]) != true ||
			is_char_escaped(line, i_tmp) == true))
		i_tmp++;
	token->type = WORD;
	token->value.str = ft_strsub(line, *i, i_tmp - (*i));
	if (token->value.str == NULL)
		token->type = ERROR;
	*i = i_tmp;
	return (FUNCT_SUCCESS);
}
