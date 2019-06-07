#include "vsh.h"

static void	history_free_copy(void)
{
	int i;

	i = 0;
	while (i < HISTORY_MAX)
	{
		ft_strdel(&history_copy[i]);
		i++;
	}
	free(history_copy);
	history_copy = NULL;
}

void		history_copy_history(void)
{
	int i;

	i = 0;
	if (history_copy)
		history_free_copy();
	history_copy = (char**)ft_memalloc(sizeof(char*) * HISTORY_MAX);
	while (i < history_cur && history[i])
	{
		history_copy[i] = ft_strdup(history[i]);
		i++;
	}
	history_copy[history_cur] = ft_strnew(0);
}
