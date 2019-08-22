/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   vsh.h                                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: omulder <omulder@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/04/10 20:29:42 by jbrinksm       #+#    #+#                */
/*   Updated: 2019/08/22 15:41:03 by omulder       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef VSH_H
# define VSH_H
// # define DEBUG
# include <sys/stat.h>
# include <fcntl.h>

/*
**==================================defines=====================================
*/

# define SHELL				"vsh"
# define FUNCT_FAILURE 0
# define FUNCT_SUCCESS 1
# define FUNCT_ERROR -1
# define PROG_FAILURE 1
# define PROG_SUCCESS 0
# define NEW_PROMPT -1
# define U_ALIAS			"alias: usage: alias [-p] [name[=value] ... ]\n"
# define U_CD				"cd: usage: cd [-L|-P] [dir]\n"
# define U_EXPORT			"export: usage: export [-n] [name[=value] ...] or export -p"
# define U_HASH				"hash: usage: hash [-r] [utility ...]\n"
# define U_UNALIAS			"unalias: usage: unalias [-a] name [name ...]\n"
# define U_SET				"set: usage: set\n"
# define E_SYNTAX_P			SHELL ": syntax error near unexpected token '%s'\n"
# define E_P_NOT_VAL_HERE	SHELL ": '%s' is not a valid heredoc delimiter\n"
# define E_P_BAD_FD			SHELL ": %s: bad file descriptor\n"
# define E_FAIL_DUP_FD		SHELL ": failed to duplicate file descriptor\n"
# define E_FD_CLOSE			SHELL ": failed to close file descriptor\n"
# define E_NO_PERM_NO_SUCH	SHELL ": no perm / no such file or directory\n"
# define E_NO_SUCH_P		SHELL ": no such file or directory: %s\n"
# define E_P_IS_DIR			SHELL ": %s: is a directory\n"
# define E_P_BAD_SUBS		SHELL ": %.*s: bad substitution\n"
# define E_P_CMD_NOT_FOUND	SHELL ": %s: command not found.\n"
# define E_FAIL_OPEN_P		SHELL ": failed to open/create %s\n"
# define E_FAIL_EXEC_P		SHELL ": failed to execute %s\n"
# define E_NO_PIPE			SHELL ": unable to create pipe"
# define E_P_BAD_REDIR		SHELL ": %s: bad redirect\n"
# define E_N_P_INV_OPT		SHELL ": %s: -%c: invalid option\n"
# define E_ALLOC_STR		SHELL ": failed to allocate enough memory\n"
# define E_N_ALLOC_STR		SHELL ": %s: failed to allocate enough memory\n"
# define E_N_PER_DEN		SHELL ": %s: permission denied\n"
# define E_N_PER_DEN_P		SHELL ": %s: permission denied: %s\n"
# define E_N_P_NOT_FOUND	SHELL ": %s: %s: not found\n"
# define E_N_P_NUM_REQ		SHELL ": %s: %s: numeric argument required\n"
# define E_N_TOO_MANY		SHELL ": %s: too many arguments\n"
# define E_N_P_NOT_VAL_ID	SHELL ": %s: '%s': not a valid identifier\n"
# define E_N_FAIL_HOME		SHELL ": %s: failed to get home directory\n"
# define E_NOT_CUR_DIR		SHELL ": cannot get current working directory\n"
# define E_NOT_RESET		SHELL ": could not reset terminal settings\n"
# define E_STAT_STR			SHELL ": could not get stat info of file\n"
# define E_STAT_P			SHELL ": could not get stat info of %s\n"
# define E_ALLOC_STR		SHELL ": failed to allocate enough memory\n"
# define E_READ_STR			SHELL ": failed to read input\n"
# define E_FORK_STR			SHELL ": fork failed\n"
# define E_HOME_NOTSET_STR 	SHELL ": environment value HOME not set\n"
# define E_HIST_READ_STR 	SHELL ": failed to read history file\n"
# define E_HIST_OPEN_STR 	SHELL ": failed to open / create history file\n"
# define E_ALIAS_OPEN_STR 	SHELL ": failed to open alias file\n"
# define E_ALIAS_READ_STR	SHELL ": failed to read alias file\n"
# define E_ALIAS_INV_NAME	SHELL ": alias: `%.*s': invalid alias name\n"
# define E_CD_CNG_DIR		SHELL ": cd: could not get current working directory parsing: %s\n"
# define E_CD_NO_SUCH		SHELL ": cd: no such file or directory: %s\n"
# define E_CD_NOT_DIR		SHELL ": cd: not a directory: %s\n"
# define E_CD_P_NOT_SET		SHELL ": cd: %s: not set\n"
# define E_TERM_CNT_GET		SHELL ": couldn't get terminal attributes.\n"
# define E_TERM_NOT_SET		SHELL ": term environment variable not set.\n"
# define E_TERM_DB_NOT_F	SHELL ": terminfo database could not be found.\n"
# define E_TERM_NO_SUCH		SHELL ": no such TERM entry in the database\n"
# define E_ALLOC 42
# define E_DUP 100
# define E_OPEN 101
# define E_BADFD 102
# define E_CLOSE 103
# define E_BADRED 104
# define CTRLD -1
# define CR 0

/*
**=================================exit codes===================================
*/

# define EXIT_WRONG_USE 2
# define EXIT_NOT_EXECUTABLE 126
# define EXIT_NOTFOUND 127
# define EXIT_FATAL 128

/*
**================================shell colors==================================
*/

# define RESET		"\033[0m"
# define RED		"\033[1;31m"
# define YEL		"\033[1;33m"
# define BLU		"\033[1;36m"

/*
**------------------------------------echo--------------------------------------
*/

# define ECHO_OPT_EL		(1 << 0)
# define ECHO_OPT_EU		(1 << 1)
# define ECHO_OPT_NL		(1 << 2)
# define BS					8
# define ESC				27

/*
**-----------------------------------export-------------------------------------
*/

# define EXP_FLAG_LN	(1 << 0)
# define EXP_FLAG_LP	(1 << 1)

/*
**-----------------------------------export-------------------------------------
*/

# define STATE_CMD	(1 << 0)
# define STATE_VAR	(1 << 1)
# define STATE_FILE	(1 << 2)

/*
**-----------------------------------alias--------------------------------------
*/

# define ALIAS_FLAG_LP		(1 << 0)
# define UNALIAS_FLAG_LA	(2 << 0)
# define ALIASFILENAME		".vsh_alias"
# define ALIAS_MAX	500


/*
**-----------------------------------hash--------------------------------------
*/

# define HT_SIZE			100
# define HT_EMPTY			0
# define HT_HAS_CONTENT		1
# define HASH_LR			(1 << 0)
# define HASH_HIT			1
# define HASH_NO_HIT		0

/*
**-----------------------------------builtin------------------------------------
*/

# define BUILTIN_CD_UL		(1 << 0)
# define BUILTIN_CD_UP		(2 << 0)

/*
**------------------------------------lexer-------------------------------------
*/

# define CURRENT_CHAR (scanner->str)[scanner->str_index]
# define SCANNER_CHAR scanner.str[scanner.str_index]
# define T_FLAG_HASSPECIAL (1 << 0)
# define T_STATE_SQUOTE (1 << 1)
# define T_STATE_DQUOTE (1 << 2)
# define T_FLAG_ISASSIGN (1 << 3)
# define T_MALLOC_ERROR (1 << 4)

/*
**-----------------------------------executor-----------------------------------
*/

# define EXEC_PIPE (1 << 0)
# define EXEC_BG (1 << 1)
# define EXEC_AND_IF (1 << 2)
# define EXEC_OR_IF (1 << 3)
# define EXEC_SEMICOL (1 << 4)

/*
**--------------------------------redirections----------------------------------
*/

# define FD_UNINIT				-1
# define REG_PERM				S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH
# define SGREAT_OPEN_FLAGS		O_WRONLY | O_CREAT | O_TRUNC
# define DGREAT_OPEN_FLAGS		O_WRONLY | O_CREAT | O_APPEND


/*
**---------------------------------environment----------------------------------
*/


# define ENV_MASK 0xF8
# define ENV_TMP_OVERWRITE (1 << 4)
# define ENV_SPECIAL (1 << 3)
# define ENV_EXTERN (1 << 2)
# define ENV_LOCAL (1 << 1)
# define ENV_TEMP (1 << 0)
# define ENV_HEAD 0

/*
**------------------------------------parser------------------------------------
*/

# define TK_TYPE (*token_lst)->type

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
# define INPUT_BACKSPACE	127

/*
**=================================pipe defines=================================
*/

# define PIPE_UNINIT	-42
# define PIPE_START		0
# define PIPE_EXTEND	1
# define PIPE_READ		0
# define PIPE_WRITE		1

/*
**----------------------------------history-------------------------------------
*/

# define HISTORY_MAX	500
# define ARROW_UP	    1
# define ARROW_DOWN	    2
# define HISTFILENAME	".vsh_history"
# define HIST_SEPARATE	-1


#define AUTO_STATE_LINE	(1 << 2)


/*
**===============================personal headers===============================
*/

# include "libft.h"

/*
**==================================headers=====================================
*/

# include <stdbool.h>

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

/*
**=================================typedefs====================================
*/

typedef struct	s_state
{
	int				exit_code;
}				t_state;

t_state *g_state;

/*
**---------------------------------environment----------------------------------
*/

typedef struct	s_envlst
{
	char			*var;
	unsigned char	type;
	struct s_envlst	*next;
}				t_envlst;

/*
**-----------------------------------history------------------------------------
*/

typedef struct	s_history
{
	int		number;
	char	*str;
}				t_history;

/*
**------------------------------------alias-------------------------------------
*/

typedef struct	s_aliaslst
{
	char				*var;
	struct s_aliaslst	*next;
}				t_aliaslst;

/*
**------------------------------------hashtable----------------------------------
*/

typedef struct	s_ht
{
	char			*key;
	char			*path;
	int				hits;
	struct s_ht		*next;
}				t_ht;

/*
**-----------------------------------term---------------------------------------
*/

typedef struct termios	t_termios;

typedef struct	s_term
{
	t_termios	*old_termios_p;
	t_termios	*termios_p;
}				t_term;

/*
**-----------------------------------vsh_data-----------------------------------
*/

typedef struct	s_vshdata
{
	t_envlst	*envlst;
	t_history	**history;
	t_aliaslst	*aliaslst;
	t_term		*term;
	int			stdfds[3];
	char		*history_file;
	char		*alias_file;
	char		*line;
	int			prompt_len;
	t_ht		*ht[HT_SIZE];
	char		ht_flag;
}				t_vshdata;

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

typedef enum	e_tokens
{
	ERROR,
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
	END
}				t_tokens;

typedef struct	s_tokenlst
{
	t_tokens			type;
	int					flags;
	char				*value;
	struct s_tokenlst	*next;
}				t_tokenlst;

typedef struct	s_scanner
{
	t_tokens	tk_type;
	int			tk_len;
	char		*str;
	int			str_index;
	char		flags;
}				t_scanner;

/*
**----------------------------------parser--------------------------------------
*/

typedef struct	s_ast
{
	t_tokens		type;
	char			flags;
	char			*value;
	struct s_ast	*left;
	struct s_ast	*right;
}				t_ast;

/*
**----------------------------------autocomplete--------------------------------
*/

typedef struct	s_print
{
	int	row;
	int	length;
	int	coli;
	int	total;
	int	printed;
	int	extra;
	int	col;
	int	left;

}				t_print;

/*
**----------------------------------pipes---------------------------------------
*/

typedef struct	s_pipes
{
	int			pipeside;
	int			parentpipe[2];
	int			currentpipe[2];
}				t_pipes;

/*
**---------------------------------environment----------------------------------
*/

char			*env_getvalue(char *var_key, t_envlst *envlst);
char			**env_free_and_return_null(char ***vshenviron);

/* environment branch -jorn */
t_envlst	*env_getlst(void);
void		env_lstaddback(t_envlst **lst, t_envlst *new);
t_envlst	*env_lstnew(char *var, unsigned char type);
char		**env_lsttoarr(t_envlst *lst);
int			env_lstlen(t_envlst *lst);
void		env_lstdel(t_envlst **envlst);
void   		env_remove_tmp(t_envlst *env);
void		env_sort(t_envlst *head);
void		env_lstadd_to_sortlst(t_envlst *envlst, t_envlst *new);
int			env_add_extern_value(t_vshdata *vshdata, char *name, char *value);

/*
**----------------------------------terminal------------------------------------
*/

t_term			*term_prepare(t_envlst *lst);
int				term_is_valid(t_envlst *envlst);
t_term			*term_init_struct(void);
int				term_get_attributes(int fd, t_term *term_p);
int				term_set_attributes(t_term *term_p);
int				term_reset(t_term *term_p);
void			term_free_struct(t_term **term_p);

/*
**-----------------------------------input--------------------------------------
*/

# define TERMCAPBUFFSIZE 12
# define TC_UP_ARROW "\e[A"
# define TC_CTRL_UP_ARROW "\e[1;5A"
# define TC_DOWN_ARROW "\e[B"
# define TC_CTRL_DOWN_ARROW "\e[1;5B"
# define TC_LEFT_ARROW "\e[D"
# define TC_CTRL_LEFT_ARROW "\e[1;5D"
# define TC_RIGHT_ARROW "\e[C"
# define TC_CTRL_RIGHT_ARROW "\e[1;5C"

# define TC_GETCURSORPOS "\e[6n"
# define TC_HOME "\e[H"
# define TC_END "\e[F"
# define TC_DELETE "\e[3~"
# define CURS_LEFT "\e[D"
# define CURS_RIGHT "\e[C"
# define CURS_UP "\e[A"
# define CURS_DOWN "\e[B"
# define INPUT_CTRL_C '\3'
# define INPUT_CTRL_D '\4'
# define INPUT_CTRL_K '\v'
# define INPUT_TAB '\t'

typedef struct	s_inputdata
{
	char		c;
	int			hist_index;
	int			hist_start;
	int			hist_first;
	unsigned	index;
	int			len_max;
	unsigned	len_cur;
	t_history	**history;
	int			tab_pressed;
}				t_inputdata;

int				input_read(t_vshdata *vshdata);
int				input_is_word_start(char *str, int i1, int i2);
void			input_clear_char_at(char **line, unsigned index);
int				input_parse_char(t_inputdata *data, t_vshdata *vshdata);
int				get_cursor_linepos(void); //column
int				get_cursor_rowpos(void); //row

int				ft_tputchar(int c);
int				tools_isprintnotblank(int i);

int				input_handle_backspace(t_inputdata *data, t_vshdata *vshdata);
int				input_handle_delete(t_inputdata *data, t_vshdata *vshdata);

void			curs_move_left(t_inputdata *data);
void			curs_move_n_left(t_inputdata *data, size_t n);

void			curs_move_right(t_inputdata *data);
void			curs_move_n_right(t_inputdata *data, t_vshdata *vshdata, size_t n);

void			curs_move_up(t_inputdata *data, t_vshdata *vshdata);
void			curs_move_down(t_inputdata *data, t_vshdata *vshdata);

int				curs_go_home(t_inputdata *data);
int				curs_go_end(t_inputdata *data, t_vshdata *vshdata);

void			curs_move_next_word(t_inputdata *data, t_vshdata *vshdata);
void			curs_move_prev_word(t_inputdata *data, t_vshdata *vshdata);

int				input_parse_ctrl_c(t_inputdata *data, t_vshdata *vshdata);
int				input_parse_ctrl_d(t_inputdata *data, t_vshdata *vshdata);
int				input_parse_ctrl_k(t_inputdata *data, t_vshdata *vshdata);
int				input_parse_tab(t_inputdata *data, t_vshdata *vshdata);
// int				input_parse_next(t_inputdata *data, char **line);
// int				input_parse_prev(t_inputdata *data, char **line);
// int				input_parse_ctrl_up(t_inputdata *data, char **line);
// int				input_parse_ctrl_down(t_inputdata *data, char **line);
// int				input_parse_ctrl_k(t_inputdata *data, char **line);

/*
**----------------------------------shell---------------------------------------
*/

void			shell_display_prompt(t_vshdata *vshdata);
int				shell_dless_read_till_stop(char **heredoc, char *stop,
					t_vshdata *vshdata);
int				shell_dless_set_tk_val(t_tokenlst *probe, char **heredoc,
					char *stop, t_vshdata *vshdata);
int				shell_dless_input(t_vshdata *vshdata, t_tokenlst **token_lst);
int				shell_close_unclosed_quotes(t_vshdata *vshdata);
int				shell_init_files(t_vshdata *vshdata);
int				shell_start(t_vshdata *vshdata);
int				shell_init_vshdata(t_vshdata *vshdata);
char			*shell_getcurrentdir(char *cwd);
int				shell_close_quote_and_esc(t_vshdata *vshdata);
char			shell_quote_checker_find_quote(char *line);
int				shell_handle_escaped_newlines(t_vshdata *vshdata);

/*
**----------------------------------lexer---------------------------------------
*/

int				lexer_tokenlstaddback(t_tokenlst **token_lst, t_tokens type,
					char *value, int flags);
t_tokenlst		*lexer_tokenlstnew(t_tokens type, char *value, int flags);
void			lexer_tokenlstdel(t_tokenlst **token_lst);
void			lexer_tokenlstiter(t_tokenlst *token_lst,
					void (*f)(t_tokenlst *elem));
bool			lexer_is_shellspec(char c);

int				lexer(char **line, t_tokenlst **token_lst);
int				lexer_error(char **line);
void			lexer_evaluator(t_tokenlst *token_lst);
int				lexer_scanner(char *line, t_tokenlst *token_lst);

void			lexer_change_state(t_scanner *scanner,
					void (*lexer_state_x)(t_scanner *scanner));
void			lexer_state_start(t_scanner *scanner);
void			lexer_state_pipe(t_scanner *scanner);
void			lexer_state_orif(t_scanner *scanner);
void			lexer_state_sgreat(t_scanner *scanner);
void			lexer_state_dgreat(t_scanner *scanner);
void			lexer_state_sless(t_scanner *scanner);
void			lexer_state_dless(t_scanner *scanner);
void			lexer_state_bg(t_scanner *scanner);
void			lexer_state_andif(t_scanner *scanner);
void			lexer_state_semicol(t_scanner *scanner);
void			lexer_state_newline(t_scanner *scanner);
void			lexer_state_squote(t_scanner *scanner);
void			lexer_state_dquote(t_scanner *scanner);
void			lexer_state_dquote_esc(t_scanner *scanner);
void			lexer_state_word(t_scanner *scanner);
void			lexer_state_word_esc(t_scanner *scanner);
void			lexer_state_lessand(t_scanner *scanner);
void			lexer_state_greatand(t_scanner *scanner);
void			lexer_state_ionum(t_scanner *scanner);

/*
**----------------------------------alias---------------------------------------
*/

int				alias_expansion(t_vshdata *vhsdata, t_tokenlst **tokenlst, char **expanded_aliases);
int				alias_replace(t_vshdata *vshdata, t_tokenlst *probe, char *alias, char **expanded_aliases);
int				alias_error(char **line, t_tokenlst **tokenlst, char ***expanded);
int				alias_read_file(t_vshdata *vshdata);
char			**alias_add_expanded(char **expanded, char *alias, char *alias_equal);
char			*alias_getvalue(char *var_key, t_aliaslst *aliaslst);


/*
**----------------------------------parser--------------------------------------
*/
int				parser_start(t_tokenlst **token_lst, t_ast **ast);
bool			parser_add_astnode(t_tokenlst **token_lst, t_ast **ast);
bool			parser_add_right(t_tokenlst **token_lst, t_ast **ast,
				bool (*parse_priority_x)(t_tokenlst **, t_ast **));
t_ast			*parser_new_node(t_tokenlst *token);
bool			parser_command(t_tokenlst **token_lst, t_ast **ast);
char			*parser_return_token_str(t_tokens type);
void			parser_astdel(t_ast **ast);
bool			parser_return_del(t_ast **ast);
bool			parser_io_redirect(t_tokenlst **token_lst, t_ast **ast);
bool			parser_cmd_param(t_tokenlst **token_lst, t_ast **cmd,
				t_ast **last_cmd_arg, t_ast **last_prefix);
bool			parser_cmd_suffix(t_tokenlst **token_lst, t_ast **cmd,
				t_ast **last_cmd_arg, t_ast **last_prefix);

/*
**----------------------------------builtins------------------------------------
*/

void			builtin_hash(char **args, t_vshdata *vshdata);
void			builtin_exit(char **args, t_vshdata *vshdata);
void			builtin_echo(char **args);
char			builtin_echo_set_flags(char **args, int *arg_i);
void			builtin_export(char **args, t_vshdata *vshdata);
void			builtin_export_var_to_type(char *varname, t_envlst *envlst, int type);
void			builtin_export_print(t_envlst *envlst, int flags);
void			builtin_export_args(char **args, t_vshdata *vshdata, int i);
int				builtin_assign(char *arg, t_vshdata *vshdata, int env_type);
int				builtin_assign_addexist(t_envlst *envlst, char *var, int env_type);
int				builtin_assign_addnew(t_envlst *envlst, char *var, int env_type);
void			builtin_set(char **args, t_envlst *envlst);
void			builtin_unset(char **args, t_envlst *envlst);
void			builtin_alias(char **args, t_aliaslst **aliaslst);
int				builtin_alias_set(char *arg, t_aliaslst **aliaslst);
void			builtin_alias_delnode(t_aliaslst **node);
void			builtin_alias_lstdel(t_aliaslst **lst);
void			builtin_unalias(char **args, t_aliaslst **aliaslst);
void			builtin_type(char **args, t_envlst *envlst, t_aliaslst *aliaslst);
int				builtin_cd(char **args, t_vshdata *vshdata);
void			builtin_cd_create_newpath(char **newpath, char *argpath);
int				builtin_cd_change_dir(char *argpath, t_vshdata *vshdata,
					char cd_flag, int print);
char			*builtin_cd_create_newpath_wrap(char *currpath, char *argpath);
int				cd_print_usage(void);
int				cd_change_dir_error(char *realpath, char *argpath,
					char **newpath, char **currpath);
int				cd_alloc_error(void);
int				cd_invalid_option(char c);

/*
**---------------------------------tools----------------------------------------
*/

bool			tool_is_redirect_tk(t_tokens type);
bool			tools_is_char_escaped(char *line, int i);
int				tools_update_quote_status(char *line, int cur_index,
					char *quote);
bool			tool_is_redirect_tk(t_tokens type);
bool			tools_isidentifierchar(char c);
bool			tools_is_valid_identifier(char *str);
bool			tools_is_builtin(char *exec_name);
bool			tools_is_fdnumstr(char *str);
bool			tool_is_special(char c);
bool			tool_check_for_special(char *str);
bool			tool_check_for_whitespace(char *str);
int				tool_get_paths(t_envlst *envlst, char ***paths);

/*
**----------------------------------execution-----------------------------------
*/

int				exec_complete_command(t_ast *ast, t_vshdata *vshdata);
int				exec_list(t_ast *ast, t_vshdata *vshdata);
int				exec_and_or(t_ast *ast, t_vshdata *vshdata);
int				exec_pipe_sequence(t_ast *ast, t_vshdata *vshdata, t_pipes pipes);
int				exec_command(t_ast *ast, t_vshdata *vshdata, t_pipes pipes);


void			exec_cmd(char **args, t_vshdata *vshdata);
bool			exec_builtin(char **args, t_vshdata *vshdata);
void			exec_external(char **args, t_vshdata *vshdata);
int				exec_find_binary(char *filename, t_vshdata *vshdata, char **binary);
int				find_binary(char *filename, t_envlst *envlst, char **binary);
void			exec_quote_remove(t_ast *node);
int				exec_validate_binary(char *binary);
int    			exec_create_files(t_ast *ast);

void			signal_print_newline(int signum);

/*
**------------------------------------expan-------------------------------------
*/

int				expan_handle_variables(t_ast *node, t_envlst *envlst);
int				expan_handle_bracketed_var(char **value, int *i, t_envlst *envlst);
int				expan_handle_dollar(char **value, int *i, t_envlst *envlst);
int				expan_tilde_expansion(t_ast *node, int *i);
int				expan_var_error_print(char *str, int len);

/*
**------------------------------------redir-------------------------------------
*/

int				redir(t_ast *node);
int				redir_output(t_ast *node);
int				redir_input(t_ast *node);
bool			redir_is_open_fd(int fd);
int				redir_input_closefd(int left_side_fd);
void			redir_change_if_leftside(t_ast *node, int *left_side_fd,
					char **right_side);
int				redir_create_heredoc_fd(char *right_side);

t_pipes			redir_init_pipestruct(void);
int				redir_pipe(t_ast *pipe_node);
int				redir_run_pipesequence(t_ast *pipenode, t_vshdata *vshdata,
					t_pipes pipes);
int				redir_handle_pipe(t_pipes pipes);

int				redir_save_stdfds(t_vshdata *vshdata);
int				return_and_reset_fds(int retval, t_vshdata *vshdata);
int				redir_reset_stdfds(t_vshdata *vshdata);
int				redir_close_saved_stdfds(t_vshdata *vshdata);

/*
**------------------------------------history-----------------------------------
*/

int				history_to_file(t_vshdata *vshdata);
int				history_get_file_content(t_vshdata *vshdata);
int				history_line_to_array(t_history **history, char **line);
void	        history_print(t_history **history);
int				history_change_line(t_inputdata *data, t_vshdata *vshdata,
					char arrow);
int				history_index_change_down(t_inputdata *data);
int				history_index_change_up(t_inputdata *data);

/*
**--------------------------------hashtable-------------------------------------
*/

int				hash_ht_insert(t_vshdata *vshdata, char *key, char *path, int count);
void			hash_print(t_ht **ht);
void			hash_reset(t_vshdata *vshdata);
void			hash_init(t_vshdata *vshdata);
unsigned int	hash_create_hash(char *key);
int				hash_check(t_vshdata *vshdata, char *key, char **binary);

/*
**--------------------------------error_handling--------------------------------
*/

int				error_return(int ret, int error, char *opt_str);
int				err_ret_exit(char *str, int exitcode);
void			err_void_exit(char *str, int exitcode);
int				err_ret(char *str);

/*
**--------------------------------autocomplete----------------------------------
*/

int				auto_get_cmdlst(char *match, t_envlst *envlst, t_list **matchlst);
int				auto_add_tolst(t_list **matchlst, char *filename);
int				auto_match_builtins(char *match, t_list **matchlst);
int				auto_get_filelst(char *match, char *path, t_list **matchlst);
int				auto_get_varlst(char *match, int match_len, t_envlst *envlst, t_list **matchlst);
int				auto_find_state(char *line, int i);
int				auto_start(t_vshdata *vshdata, t_inputdata *data);
int				auto_add_match_toline(char *match, char *to_add, t_vshdata *vshdata, t_inputdata *data);
int				auto_find_matches(t_vshdata *vshdata, char **match, t_list **matchlst, int state);
void			auto_lstdel(void *str, size_t size);
int				auto_handle_matchlst(t_vshdata *vshdata, t_inputdata *data, char *match, t_list **matchlst);
int				auto_small_lst(char *match, t_list **matchlst, t_vshdata *vshdata, t_inputdata *data);
void			auto_lst_print(t_list **matchlst, int lst_len);
int				auto_big_lst(t_list **matchlst, int lst_len);
int				auto_lenname(t_list *matchlst, int length);
int				auto_lst_count(t_list *lst);
void			auto_sort_n(t_list **matchlst);
void			auto_swap_lstitem(t_list **flst, t_list *smal, t_list *prev);
void			auto_clear_line(t_inputdata *data, t_vshdata *vshdata);


/*
**----------------------------------debugging-----------------------------------
*/

void			print_node(t_tokenlst *node);
void			print_tree(t_ast *root);
void			print_token(t_scanner *scanner);
void			print_tree(t_ast *root);

#endif
