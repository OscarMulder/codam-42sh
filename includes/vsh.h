/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   vsh.h                                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: omulder <omulder@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/04/10 20:29:42 by jbrinksm       #+#    #+#                */
/*   Updated: 2019/05/21 11:47:17 by mavan-he      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef VSH_H
# define VSH_H
# define DEBUG

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

# define ECHO_OPT_EL		(1 << 0)
# define ECHO_OPT_EU		(1 << 1)
# define ECHO_OPT_NL		(1 << 2)
# define BS					8
# define ESC				27

/*
**------------------------------------lexer-------------------------------------
*/

# define CURRENT_CHAR (scanner->str)[scanner->str_index]

/*
**-----------------------------------input--------------------------------------
*/

# define INPUT_NONE			0
# define INPUT_ESC			1
# define INPUT_BRACE		2
# define INPUT_THREE		3
# define INPUT_D_ESC		4
# define INPUT_D_BRACE		5
# define INPUT_D_THREE		6

/*
**===============================personal headers===============================
*/

# include "libft.h"

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
**=================================typedefs====================================
*/

typedef struct		s_term
{
	struct termios	*old_termios_p;
	struct termios	*termios_p;
}					t_term;

/*
**----------------------------------lexer--------------------------------------
*/
/*
**	START,
**	WORD, // bascially any string
**	ASSIGN, WORD=[WORD]
**	IO_NUMBER, // NUM followed by > or <
**	AND_IF, // &&
**	OR_IF, // ||
**	DLESS, // <<
**	DGREAT, // >>
**	SLESS, // <
**	SGREAT, // >
**	LESSAND, // <&
**	GREATAND, // >&
**	BG // & in background
**	PIPE, // |
**	SEMICOL // ;
**	NEWLINE,
**	END,
**	ERROR // malloc fail
*/
typedef enum		e_tokens
{
	START,
	WORD,
	ASSIGN,
	IO_NUMBER,
	AND_IF,
	OR_IF,
	DLESS,
	DGREAT,
	SLESS,
	SGREAT,
	LESSAND,
	GREATAND,
	BG,
	PIPE,
	SEMICOL,
	NEWLINE,
	END,
	ERROR
}					t_tokens;

typedef union		u_tk_value
{
	char			*str;
	char			**array;
	int				io;
}					t_tk_value;

typedef struct		s_token
{
	t_tokens		type;
	t_tk_value		value;
}					t_token;

typedef struct		s_scanner
{
	t_tokens		tk_type;
	int				tk_len;
	char			*str;
	int				str_index;
}					t_scanner;
/*
**=================================prototypes===================================
*/

int					term_reset(t_term *term_p);
void				term_free_termp(t_term *term_p);
int					shell_start(void);

/*
**---------------------------------environment----------------------------------
*/

char				**get_environ_cpy(void);
char				*var_get_value(char *var_key, char **vararray);
char				*var_join_key_value(char *var_key, char *var_value);
int					var_set_value(char *var_key, char *var_value,
					char **vararray);
int					var_add_value(char *var_key, char *var_value,
					char ***vararray);
char				**free_and_return_null(char ***vshenviron);

/*
**----------------------------------terminal------------------------------------
*/

t_term				*term_prepare(char **vshenviron);
t_term				*term_return(t_term *term_p, int return_value);
int					term_is_valid(char **vshenviron);
t_term				*term_init_struct(void);
int					term_get_attributes(int fd, t_term *term_p);
int					term_set_attributes(t_term *term_p);
int					term_reset(t_term *term_p);
void				term_free_struct(t_term **term_p);

/*
**-----------------------------------input--------------------------------------
*/

int					input_read(char **line);
int					input_is_word_start(char *str, int i1, int i2);
void				input_clear_char_at(char **line, unsigned index);
int					input_parse_escape(char c, int *input_state);
int					input_parse_char(char c, unsigned *index, char **line);
int					input_parse_home(char c, int *input_state,
					unsigned *index);
int					input_parse_backspace(char c, unsigned *index,
					char **line);
int					input_parse_end(char c, int *input_state,
					unsigned *index, char **line);
int					input_parse_next(char c, int *input_state,
					unsigned *index, char **line);
int					input_parse_prev(char c, int *input_state,
					unsigned *index, char **line);
int					input_parse_delete(char c, int *input_state,
					unsigned *index, char **line);
int					input_parse_ctrl_d(char c, unsigned *index, char **line);
int					input_parse_ctrl_k(char c, unsigned *index, char **line);
int					input_parse_ctrl_up(char c, int *input_state,
					unsigned *index, char **line);
int					input_parse_ctrl_down(char c, int *input_state,
					unsigned *index, char **line);

/*
**----------------------------------shell---------------------------------------
*/

void				shell_display_prompt(void);

/*
**----------------------------------lexer---------------------------------------
*/
int					lexer(char *line, t_list **token_lst);
int					add_tk_to_lst(t_list **lst, t_token *token);
void				del_tk_node(void *content, size_t size);
int					lexer_error(t_list **token_lst);
void				evaluator(t_list *token_lst);
int					lexer_scanner(char *line, t_list *token_lst);
void				change_state(t_scanner *scanner,
					void (*state_x)(t_scanner *scanner));
void				print_token(t_scanner *scanner);
void				state_1(t_scanner *scanner);
void				state_2(t_scanner *scanner);
void				state_3(t_scanner *scanner);
void				state_4(t_scanner *scanner);
void				state_5(t_scanner *scanner);
void				state_6(t_scanner *scanner);
void				state_7(t_scanner *scanner);
void				state_8(t_scanner *scanner);
void				state_9(t_scanner *scanner);
void				state_10(t_scanner *scanner);
void				state_11(t_scanner *scanner);
void				state_12(t_scanner *scanner);
void				state_13(t_scanner *scanner);
void				state_14(t_scanner *scanner);
void				state_15(t_scanner *scanner);
void				state_16(t_scanner *scanner);
void				state_17(t_scanner *scanner);
void				state_18(t_scanner *scanner);
void				state_19(t_scanner *scanner);

/*
**----------------------------------parser--------------------------------------
*/

/*
**----------------------------------bultins-------------------------------------
*/

void				builtin_exit(t_term *term_p);
int					builtin_echo(char **args);
char				echo_set_flags(char **args, int *arg_i);

/*
**---------------------------------tools----------------------------------------
*/

int					is_char_escaped(char *line, int i);
int					update_quote_status(char *line, int cur_index, char *quote);

/*
**----------------------------------debugging-----------------------------------
*/

void				print_node(t_list *node);

#endif
