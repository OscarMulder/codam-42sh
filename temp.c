/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   temp.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: omulder <omulder@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/07/26 15:43:25 by omulder        #+#    #+#                */
/*   Updated: 2019/07/26 15:44:33 by omulder       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

t_inputdata	*init_inputdata(void)
{
	t_inputdata	*new;

	new = (t_inputdata*)ft_memalloc(sizeof(t_inputdata));
	new->c = 0;
	new->index = 0;
	new->input_state = 0;
	new->hist_index = 0;
	return (new);
}

typedef struct	s_inputdata
{
	char		c;
	int			input_state;
	int			hist_index;
	unsigned	index;
}				t_inputdata;

int				input_is_word_start(char *str, int i1, int i2);
void			input_clear_char_at(char **line, unsigned index);
int				input_parse_escape(t_inputdata *data);
int				input_parse_char(t_inputdata *data, char **line, int *len_max);
int				input_parse_home(t_inputdata *data);
int				input_parse_backspace(t_inputdata *data, char **line);
int				input_parse_end(t_inputdata *data, char **line);
int				input_parse_next(t_inputdata *data, char **line);
int				input_parse_prev(t_inputdata *data, char **line);
int				input_parse_delete(t_inputdata *data, char **line);
int				input_parse_ctrl_d(t_inputdata *data, char **line);
int				input_parse_ctrl_up(t_inputdata *data, t_history **history, char **line);
int				input_parse_ctrl_down(t_inputdata *data, t_history **history, char **line);
int				input_parse_ctrl_k(t_inputdata *data, char **line);