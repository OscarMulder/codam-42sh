/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   exec_external.c                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: omulder <omulder@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/05/31 10:47:19 by tde-jong       #+#    #+#                */
/*   Updated: 2019/09/13 16:16:54 by jbrinksm      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"
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

void			signal_print_newline(int signum)
{
	(void)signum;
	ft_putchar('\n');
	signal(SIGINT, signal_print_newline);
}

static void		exec_add_pid_to_pipeseqlist(t_vshdata *data, pid_t pid)
{
	t_pipeseqlist	*probe;

	if (data->pipeseq == NULL)
	{
		data->pipeseq = ft_memalloc(sizeof(t_pipeseqlist));
		if (data->pipeseq == NULL) //WHAT DO WE DO ON FAIL
			return ;
		data->pipeseq->pid = pid;
	}
	else
	{
		probe = data->pipeseq;
		data->pipeseq = ft_memalloc(sizeof(t_pipeseqlist));
		if (data->pipeseq == NULL) //WHAT DO WE DO ON FAIL
			return ;
		data->pipeseq->pid = pid;
		data->pipeseq->next = probe;
	}
}

static void		exec_bin_handle_parent(t_vshdata *data, pid_t pid, char *binary,
bool is_pipe)
{
	jobs_add_job(data, pid, binary);
	if (is_pipe)
		exec_add_pid_to_pipeseqlist(data, pid);
	signal(SIGINT, signal_print_newline);
}

static void		exec_bin(char *binary, char **args, char **vshenviron,
t_vshdata *data, bool is_pipe)
{
	pid_t	pid;
	int		status;

	if (exec_validate_binary(binary) == FUNCT_ERROR)
		return ;
	term_flags_init(data->term->termios_p);
	pid = fork();
	if (pid < 0)
		return (err_void_exit(E_FORK_STR, EXIT_FAILURE));
	if (pid > 0)
		exec_bin_handle_parent(data, pid, binary, is_pipe);
	else
	{
		execve(binary, args, vshenviron);
		ft_eprintf(E_FAIL_EXEC_P, binary);
		exit(EXIT_FAILURE);
	}
	waitpid(pid, &status, WUNTRACED);
	if (WIFEXITED(status))
		g_state->exit_code = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		g_state->exit_code = EXIT_FATAL + WTERMSIG(status);
	signal(SIGINT, SIG_DFL);
	term_flags_destroy(data->term->termios_p);
}

void			exec_external(char **args, t_vshdata *data, bool is_pipe)
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
			exec_bin(binary, args, vshenviron, data, is_pipe);
	}
	else
		exec_bin(binary, args, vshenviron, data, is_pipe);
	free(vshenviron);
	ft_strdel(&binary);
}
