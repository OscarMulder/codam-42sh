/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   glob_matching.c                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: mavan-he <mavan-he@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/10/14 15:23:48 by mavan-he       #+#    #+#                */
/*   Updated: 2019/10/17 18:17:00 by mavan-he      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"

int			glob_matching_wild(t_globtoken *tokenprobe, t_globmatchlst match)
{
	while (match.index <= match.word_len)
	{
		if (glob_start_matching(tokenprobe->next, match) == FUNCT_SUCCESS)
			return (FUNCT_SUCCESS);
		match.index++;
	}
	return (FUNCT_FAILURE);
}

int			glob_matching_braceneg(t_globtoken *tokenprobe, t_globmatchlst match)
{
	int i;

	i = 1;
	while (tokenprobe->word_chunk[i + 1] != '\0')
	{
		if (tokenprobe->word_chunk[i] == match.word[match.index])
			return (FUNCT_FAILURE);
		i++;
	}
	match.index++;
	return (glob_start_matching(tokenprobe->next, match));
}

int			glob_matching_bracepos(t_globtoken *tokenprobe, t_globmatchlst match)
{
	int i;

	i = 1;
	while (tokenprobe->word_chunk[i + 1] != '\0')
	{
		if (tokenprobe->word_chunk[i] == match.word[match.index])
		{
			match.index++;
			return (glob_start_matching(tokenprobe->next, match));
		}
		i++;
	}
	return (FUNCT_FAILURE);
}

int			glob_quest_or_str(t_globtoken *tokenprobe, t_globmatchlst match)
{
	if (tokenprobe->tk_type == GLOB_STR)
	{
		if (ft_strnequ(tokenprobe->word_chunk, &match.word[match.index],
			tokenprobe->word_len) == false)
			return (FUNCT_FAILURE);
		else
		{
			match.index += tokenprobe->word_len;
			return (glob_start_matching(tokenprobe->next, match));
		}
	}
	else if (tokenprobe->tk_type == GLOB_QUEST)
	{
		if (match.word[match.index] == '\0')
			return (FUNCT_FAILURE);
		else
		{
			match.index += tokenprobe->word_len;
			return (glob_start_matching(tokenprobe->next, match));
		}
	}
	return (FUNCT_ERROR);
}

int			glob_start_matching(t_globtoken *tokenprobe, t_globmatchlst match)
{
	if (match.index == match.word_len &&
		(tokenprobe == NULL || tokenprobe->tk_type == GLOB_SLASH))
		return (FUNCT_SUCCESS);
	else if (tokenprobe == NULL || tokenprobe->tk_type == GLOB_SLASH)
		return (FUNCT_FAILURE);
	if (tokenprobe->tk_type == GLOB_STR || tokenprobe->tk_type == GLOB_QUEST)
		return (glob_quest_or_str(tokenprobe, match));
	else if (tokenprobe->tk_type == GLOB_BRACEPOS)
		return (glob_matching_bracepos(tokenprobe, match));
	else if (tokenprobe->tk_type == GLOB_BRACENEG)
		return (glob_matching_braceneg(tokenprobe, match));
	else if (tokenprobe->tk_type == GLOB_WILD)
		return (glob_matching_wild(tokenprobe, match));
	return (FUNCT_SUCCESS);
}
