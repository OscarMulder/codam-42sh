/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   vsh.h                                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: omulder <omulder@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/04/10 20:29:42 by jbrinksm       #+#    #+#                */

/*   Updated: 2019/04/23 16:54:55 by jbrinksm      ########   odam.nl         */

/*                                                                            */
/* ************************************************************************** */

#ifndef VSH_H
# define VSH_H

# define CMD_LIST t_list
# define ARG_LIST t_list

/*
**==================================defines=====================================
*/

# define FUNCT_FAILURE 0
# define FUNCT_SUCCESS 1
# define FUNCT_ERROR -1
# define E_ALLOC 420
# define CTRLD -1
# define CR 0

/*
**------------------------------------echo--------------------------------------
*/

# define OPT_E		(1<<0)
# define OPT_CE		(1<<1)
# define OPT_N		(1<<2)

/*
**===============================personal headers===============================
*/

#include "libft.h"

/*
**==================================headers=====================================
*/

# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include <dirent.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <signal.h>

# include <sys/ioctl.h>
# include <termios.h>
# include <curses.h>
# include <term.h>
# include <fcntl.h>
# include <sys/types.h>
# include <sys/uio.h>
# include <signal.h>

/*
**	malloc, free, close, fork, execve, exit | getenv
**	access, write, read, getcwd, chdir | isatty ttyname ttyslot write close
**	open
**	opendir readdir closedir
**	stat lstat fstat
**	wait, wait3, wait4, waitpid
**	signal kill
**	****************************************************************************
**	ioctl
**	tcsetattr tcgetattr
**
**	tgetent tgetflag tgetnum tgetstr tgoto tputs
**	open
**
**	read
**	signal
*/

# include <sys/param.h>

/*
**=================================structs======================================
*/

typedef struct	s_term
{
	struct termios	*old_termios_p;
	struct termios	*termios_p;
}				t_term;

/*
**=================================prototypes===================================
*/

int		term_reset(t_term *term_p);
void	term_free_termp(t_term *term_p);
int		shell_start(void);

/*
**---------------------------------environment----------------------------------
*/

char	**get_environ_cpy(void);
char	*var_get_value(char *var_key, char **vararray);
char	*var_join_key_value(char *var_key, char *var_value);
int		var_set_value(char *var_key, char *var_value, char **vararray);
char	**free_and_return_null(char ***vshenviron);

/*
**----------------------------------terminal------------------------------------
*/

t_term	*term_prepare(char **vshenviron);
t_term	*term_return(t_term *term_p, int return_value);
int		term_is_valid(char **vshenviron);
t_term	*term_init_struct(void);
int		term_get_attributes(int fd, t_term *term_p);
int		term_set_attributes(t_term *term_p);
int		term_reset(t_term *term_p);
void	term_free_struct(t_term **term_p);

/*
**-----------------------------------input--------------------------------------
*/

int		input_read(char **line);
int		input_echo(char *buf);

/*
**----------------------------------shell---------------------------------------
*/

void	shell_display_prompt(void);

/*
**----------------------------------parser--------------------------------------
*/

int		parser_lexer(char *line, CMD_LIST **cmd_tab);

int		is_uninhibited_semicolon(char *str, int i, char quote);
int		is_uninhibited_blank(char *str, int i);

t_list	*parser_split_line_to_commands(char *line);
void	parser_add_command_to_lst(char *command, ARG_LIST **args);
int		parser_strlen_cmd(char *line);

void	parser_remove_quotes(t_list *cmdstr_lst);
void	parser_rem_esc_char_quotes(t_list *cmdstr_lst);
void	parser_rem_esc_char_semicolons(t_list *cmdstr_lst);

t_list	*parser_split_command_to_args(char *cmd);

/*
**----------------------------------bultins-------------------------------------
*/

void	builtin_exit(t_term *term_p);
int		builtin_echo(char **args);
char	echo_set_flags(char **args, int *arg_i);

/*
**---------------------------------tools----------------------------------------
*/

int		is_char_escaped(char *line, int cur_index);
int		update_quote_status(char *line, int cur_index, char *quote);

#endif
