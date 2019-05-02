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

/*
**==================================defines=====================================
*/

# define FUNCT_FAILURE 0
# define FUNCT_SUCCESS 1
# define FUNCT_ERROR -1
# define E_ALLOC 420
# define CTRLD -1
# define CR 0

# define VAR_READONLY	(1<<0)
# define VAR_INTERN		(1<<1)

/*
**------------------------------------echo--------------------------------------
*/

# define OPT_E		(1<<0)
# define OPT_CE		(1<<1)
# define OPT_N		(1<<2)

/*
**-------------------------------------cd---------------------------------------
*/

# define CD_OPT_LL	0
# define CD_OPT_LP	1

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

typedef struct	s_env_var
{
	char		*data;
	char		flags:2;
}				t_env_var;

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
char	*param_to_env(char *parameter, char **vshenviron);
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

int		parser_lexer(char *line, char ***commands);

char	**parser_split_line_to_commands(char *line);
char	*parser_strdup_command_from_line(char *line, int *start_arg_index);
int		parser_command_len_from_line(char *line, int *start_arg_index);
int		parser_total_commands_from_line(char *line);

/*
**----------------------------------bultins-------------------------------------
*/

void	builtin_exit(t_term *term_p);
int		builtin_echo(char **args);
int		builtin_cd(char **args, char **env);
char	echo_set_flags(char **args, int *arg_i);

/*
**---------------------------------tools----------------------------------------
*/

int		is_char_escaped(char *line, int cur_index);
int		update_quote_status(char *line, int cur_index, char *quote);

/*
**==================================TEST PROTOTYPES=============================
*/

/*
**----------------------------------environment_testers-------------------------
*/

int		test_get_environ_cpy(void);
int		test_param_to_env(void);
int		test_free_and_return_null(void);

/*
**--------------------------------term_testers----------------------------------
*/

int		test_term_is_valid(void);
int		test_term_init_struct(void);
int		test_term_free_struct(void);
int		test_term_get_attributes(void);

/*
**--------------------------------prompt_testers--------------------------------
*/

int		test_prompt(void);

/*
**--------------------------------test_parser_lexer-----------------------------
*/

int		test_parser_split_line_to_commands(void);
int		test_parser_strdup_command_from_line(void);
int		test_parser_total_commands_from_line(void);
int		test_parser_command_len_from_line(void);


/*
**--------------------------------tools_testers---------------------------------
*/

int		test_is_char_escaped(void);
int		test_update_quote_status(void);

/*
**-----------------------------------builtins-----------------------------------
*/

int		test_echo(void);

#endif
