/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   shell_getcurrentdir.c                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: jbrinksm <jbrinksm@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/07/30 17:27:41 by jbrinksm       #+#    #+#                */
/*   Updated: 2019/07/30 17:36:56 by jbrinksm      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"
#include <unistd.h>
#include <limits.h>

#define STR 0
#define Y 1
#define X 2

static void	make_word(const char *s, char c, char **array, int *index)
{
	if (index[STR] == 0 && s[index[STR]] == c)
	{
		array[index[Y]][index[X]] = c;
		array[index[Y]][index[X] + 1] = '\0';
	}
	else
	{
		while (s[index[STR]] == c)
			index[STR]++;
		if (index[STR] == 1 && s[0] == c)
			array[index[Y]][index[X]] = c;
		if (index[STR] == 1 && s[0] == c)
			index[X]++;
		while (s[index[STR]] != c && s[index[STR]] != '\0')
		{
			array[index[Y]][index[X]] = s[index[STR]];
			index[X]++;
			index[STR]++;
		}
		array[index[Y]][index[X]] = '\0';
	}
	index[Y]++;
	index[STR]++;
	index[X] = 0;
	array[index[Y]] = NULL;
}

static int	ft_countwords(const char *str, char delimiter)
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

static char	**return_null(char ***array)
{
	ft_strarrdel(array);
	return (NULL);
}

static char	**ft_getdirarraynoslash(const char *str, char c)
{
	int		index[3];
	char	**array;
	int		words;
	int		word_length;

	if (!str)
		return (NULL);
	words = ft_countwords(str, c);
	index[STR] = 0;
	index[Y] = 0;
	index[X] = 0;
	array = (char**)malloc(sizeof(char*) * (words + 1));
	if (array == NULL)
		return (NULL);
	while (str[index[STR]] != '\0')
	{
		word_length = ft_strclen(&str[index[STR]], c);
		array[index[Y]] = ft_strnew(word_length);
		if (array[index[Y]] == NULL)
			return (return_null(&array));
		make_word(str, c, array, index);
	}
	return (array);
}

char		*shell_getcurrentdir(void)
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
