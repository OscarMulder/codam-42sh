/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   exec_cmd.c                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: omulder <omulder@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/05/29 17:17:48 by omulder        #+#    #+#                */
/*   Updated: 2019/09/09 19:39:17 by jbrinksm      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"

// #include "vsh.h"
#include <unistd.h>
#include <sys/wait.h>
#include <termios.h>
#include <signal.h>

static void		term_flags_init(t_termios *termios_p)
{
	termios_p->c_lflag |= (ECHO | ICANON | ISIG);
	tcsetattr(STDIN_FILENO, TCSANOW, termios_p);
}

static void		term_flags_destroy(t_termios *termios_p)
{
	termios_p->c_lflag &= ~(ECHO | ICANON | ISIG);
	tcsetattr(STDIN_FILENO, TCSANOW, termios_p);
}

static void		exec_bin(char *binary, char **args, char **vshenviron,
t_vshdata *data)
{
	pid_t	pid;
	t_list	*save_pid;

	if (exec_validate_binary(binary) == FUNCT_ERROR)
		return ;
	term_flags_init(data->term->termios_p);
	pid = fork();
	save_pid = ft_lstnew(exec_get_pid_ptr(pid), sizeof(pid_t));
	if (pid < 0 || save_pid == NULL)
		return (err_void_exit(E_FORK_STR, EXIT_FAILURE));
	ft_lstaddback(data->pids, save_pid);
	if (pid > 0)
		signal(SIGINT, signal_print_newline);
	else
	{
		ft_eprintf("temp executing: %s\n", binary);
		execve(binary, args, vshenviron);
		ft_eprintf(E_FAIL_EXEC_P, binary);
		exit(EXIT_FAILURE);
	}
	signal(SIGINT, SIG_DFL);
	term_flags_destroy(data->term->termios_p);
	ft_eprintf("temp done executing: %s\n", binary);
}

static void			exec_external_nowait(char **args, t_vshdata *data)
{
	char	**vshenviron;
	char	*binary;

	binary = ft_strdup(args[0]);
	vshenviron = env_lsttoarr(data->envlst);
	if (binary == NULL || vshenviron == NULL)
	{
		ft_strdel(&binary);
		free(vshenviron);
		ft_eprintf(E_ALLOC_STR);
		g_state->exit_code = EXIT_FAILURE;
		return ;
	}
	if (args[0][0] != '/' && ft_strnequ(args[0], "./", 2) == 0 &&
		ft_strnequ(args[0], "../", 3) == 0)
	{
		ft_strdel(&binary);
		if (exec_find_binary(args[0], data, &binary) == FUNCT_SUCCESS)
			exec_bin(binary, args, vshenviron, data);
	}
	else
		exec_bin(binary, args, vshenviron, data);
	free(vshenviron);
	ft_strdel(&binary);
}

void		exec_cmd(char **args, t_vshdata *data, t_pipes pipes)
{
	if (exec_builtin(args, data) == false)
	{
		if (pipes.pipeside == PIPE_UNINIT || (pipes.pipeside == PIPE_EXTEND
		&& pipes.currentpipe[0] != PIPE_UNINIT && pipes.currentpipe[1] != PIPE_UNINIT && pipes.parentpipe[0] == PIPE_UNINIT && pipes.parentpipe[1] == PIPE_UNINIT))
			exec_external(args, data);
		else
			exec_external_nowait(args, data);
	}
	ft_strarrdel(&args);
	env_remove_tmp(data->envlst);
}
