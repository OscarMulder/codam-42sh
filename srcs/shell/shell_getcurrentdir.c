/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   shell_getcurrentdir.c                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: jbrinksm <jbrinksm@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/07/30 17:27:41 by jbrinksm       #+#    #+#                */
/*   Updated: 2019/07/30 18:08:43 by jbrinksm      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"
#include <unistd.h>
#include <sys/param.h>

char		*shell_getcurrentdir(void)
{
	char	*cwd;
	int		i;

	i = 0;
	cwd = getcwd(NULL, MAXPATHLEN);
	if (cwd == NULL)
		return (NULL);
	if (cwd[0] == '/' && cwd[0 + 1] == '\0')
		return (cwd);
	while (cwd[i] != '\0')
		i++;
	while (cwd[i] != '/')
		i--;
	i++;
	return (&cwd[i]);
}
