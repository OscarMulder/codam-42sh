/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   shell_prompt.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: jbrinksm <jbrinksm@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/04/11 20:16:38 by jbrinksm       #+#    #+#                */
/*   Updated: 2019/07/30 17:02:07 by jbrinksm      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"
#include <unistd.h>
#include <limits.h>

#define S_INDEX strsplit_indxs[0]
#define ARRAY_Y strsplit_indxs[1]
#define ARRAY_X strsplit_indxs[2]


static void	make_word(const char *s, char c, char **array, int *strsplit_indxs)
{
	if (S_INDEX == 0 && s[S_INDEX] == c)
	{
		array[ARRAY_Y][ARRAY_X] = c;
		array[ARRAY_Y][ARRAY_X + 1] = '\0';
	}
	else
	{
		while (s[S_INDEX] == c)
			S_INDEX++;
		if (S_INDEX == 1 && s[0] == c)
			array[ARRAY_Y][ARRAY_X] = c;
		if (S_INDEX == 1 && s[0] == c)
			ARRAY_X++;
		while (s[S_INDEX] != c && s[S_INDEX] != '\0')
		{
			array[ARRAY_Y][ARRAY_X] = s[S_INDEX];
			ARRAY_X++;
			S_INDEX++;
		}
		array[ARRAY_Y][ARRAY_X] = '\0';
	}
	ARRAY_Y++;
	S_INDEX++;
	ARRAY_X = 0;
	array[ARRAY_Y] = NULL;
}

static int		ft_wordlength(const char *str, char delimiter)
{
	int i;

	i = 0;
	while (str[i] != '\0' && str[i] != delimiter)
		i++;
	return (i);
}

static int		ft_countwords(const char *str, char delimiter)
{
	int i;
	int word_count;
	int word_trigger;

	i = 0;
	word_count = 0;
	word_trigger = 1;
	while (str[i] != '\0')
	{
		if (str[i] != delimiter)
		{
			if (word_trigger)
			{
				word_count++;
				word_trigger = 0;
			}
		}
		if (str[i] == delimiter)
			word_trigger = 1;
		i++;
	}
	return (word_count);
}

static char		**ft_getdirarraynoslash(const char *s, char c)
{
	int		strsplit_indxs[3];
	char	**array;
	int		words;
	int		word_length;

	if (!s)
		return (NULL);
	words = ft_countwords(s, c);
	S_INDEX = 0;
	ARRAY_Y = 0;
	ARRAY_X = 0;
	array = (char**)malloc(sizeof(char*) * (words + 2));
	if (array == NULL)
		return (NULL);
	while (s[S_INDEX] != '\0')
	{
		word_length = ft_wordlength(&s[S_INDEX], c);
		array[ARRAY_Y] = ft_strnew(word_length + 1);
		make_word(s, c, array, strsplit_indxs);
	}
	return (array);
}

static char		*get_current_dir(void)
{
	char	*cwd;
	char	**split;
	char	*dir;
	int		index;

	cwd = ft_strnew(PATH_MAX);
	cwd = getcwd(cwd, PATH_MAX);
	split = ft_getdirarraynoslash(cwd, '/');
	index = 0;
	while (split[index] != NULL)
		index++;
	if (index > 0)
		index--;
	dir = ft_strdup(split[index]);
	ft_strdel(&cwd);
	ft_strarrdel(&split);
	return (dir);
}

void	shell_display_prompt(void)
{
	char *current_dir;

	current_dir = get_current_dir();
	if (current_dir == NULL)
		ft_printf("\033[1;31vsh > \033[0m");
	else
		ft_printf("\033[1;31mvsh \033[1;36m%s \033[1;33m> \
\033[0m", current_dir);
}
