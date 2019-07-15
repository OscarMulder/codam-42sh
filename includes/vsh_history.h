/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   vsh_history.h                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: mavan-he <mavan-he@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/05/30 17:42:22 by mavan-he       #+#    #+#                */
/*   Updated: 2019/07/15 16:45:46 by omulder       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef VSH_HISTORY_H
# define VSH_HISTORY_H
# define HISTFILE "~/.vsh_history"

// int	history_i;
// int	history_cur;
// int	history_tmp;

// char	**history;
// char	**history_copy;

typedef struct  s_history
{
    int     number;
    char    *str;
}               t_history;

# define HISTORY_MAX	500
# define ARROW_UP	    1
# define ARROW_DOWN	    2

int		        history_to_file(t_history **history);
int		        history_get_file_content(t_history ***history);
int		        history_line_to_array(t_history **history, char *line);
void	        history_print(t_history **history);
void		    history_change_line(t_history **history, char **line, unsigned *index, char arrow);

#endif