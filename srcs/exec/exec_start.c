/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   exec_start.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: omulder <omulder@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/05/29 17:52:22 by omulder        #+#    #+#                */
/*   Updated: 2019/07/19 22:48:38 by jbrinksm      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"
#include "unistd.h"
#include "fcntl.h"

static size_t	count_args(t_ast *ast)
{
	t_ast	*probe;
	size_t	i;

	i = 0;
	probe = ast;
	while (probe != NULL)
	{
		i++;
		probe = probe->child;
	}
	return (i);
}

static char	**create_args(t_ast *ast)
{
	char	**args;
	t_ast	*probe;
	size_t	total_args;
	size_t	i;

	total_args = count_args(ast);
	args = (char**)ft_memalloc(sizeof(char*) * (total_args + 1));
	if (args == NULL)
		return (NULL);
	i = 0;
	probe = ast;
	while (i < total_args)
	{
		#ifdef DEBUG
		if (probe->type != WORD)
			ft_putendl("Found non-WORD item in child-flow of WORD's (aka I messed something up -Jorn)");
		#endif
		args[i] = ft_strdup(probe->value);
		if (args[i] == NULL)
		{
			ft_strarrdel(&args);
			return (NULL);
		}
		probe = probe->child;
		i++;
	}
	return (args);
}

static void	redir_input(t_ast *node, int *exit_code)
{
	(void)exit_code;
	int		fd;
	int		filefd;
	int		pipefds[2];

	if (node->type == SLESS)
	{
		fd = STDIN_FILENO;
		/* If there is a IONUM on left side of redir */
		if (node->sibling->child != NULL)
		{
			fd = ft_atoi(node->sibling->value);
			filefd = open(node->sibling->child->value, O_RDONLY);
		}
		else
			filefd = open(node->sibling->value, O_RDONLY);
		dup2(filefd, fd);
		close(filefd);
	}
	else if (node->type == DLESS)
	{
		fd = STDIN_FILENO;
		pipe(pipefds);
		/* If there is a IONUM on left side of redir */
		if (node->sibling->child != NULL)
		{
			fd = ft_atoi(node->sibling->value);
			write(pipefds[1], node->sibling->child->value, ft_strlen(node->sibling->child->value));
		}
		else
			write(pipefds[1], node->sibling->value, ft_strlen(node->sibling->value));
		close(pipefds[1]);
		filefd = pipefds[0];
		dup2(filefd, fd);
		close(filefd);
	}
	else if (node->type == LESSAND)
	{
		fd = STDIN_FILENO;
		/* If there is a IONUM on left side of redir */
		if (node->sibling->child != NULL)
		{
			fd = ft_atoi(node->sibling->value);
			filefd = ft_atoi(node->sibling->child->value);
		}
		else
			filefd = ft_atoi(node->sibling->value);
		dup2(filefd, fd);
		close(filefd);
	}
}

static void	redir_output(t_ast *node, int *exit_code)
{
	(void)exit_code;
	int		fd;
	int		filefd;

	if (node->type == SGREAT)
	{
		/* DOESNT CLEAN WHOLE FILE YET */

		fd = STDOUT_FILENO;
		/* If there is a IONUM on left side of redir */
		if (node->sibling->child != NULL)
		{
			fd = ft_atoi(node->sibling->value);
			filefd = open(node->sibling->child->value, O_WRONLY | O_CREAT);
		}
		else
			filefd = open(node->sibling->value, O_WRONLY | O_CREAT);
		dup2(filefd, fd);
		close(filefd);
	}
	else if (node->type == DGREAT)
	{
		fd = STDOUT_FILENO;
		/* If there is a IONUM on left side of redir */
		if (node->sibling->child != NULL)
		{
			fd = ft_atoi(node->sibling->value);
			filefd = open(node->sibling->child->value, O_WRONLY | O_CREAT | O_APPEND);
		}
		else
			filefd = open(node->sibling->value, O_WRONLY | O_CREAT | O_APPEND);
		dup2(filefd, fd);
		close(filefd);
	}
	else if (node->type == GREATAND)
	{
		fd = STDOUT_FILENO;
		/* If there is a IONUM on left side of redir */
		if (node->sibling->child != NULL)
		{
			fd = ft_atoi(node->sibling->value);
			filefd = ft_atoi(node->sibling->child->value);
		}
		else
			filefd = ft_atoi(node->sibling->value);
		dup2(filefd, fd);
		close(filefd);
	}
}



// }
// 	char	**args;
// 	t_ast	*probe;
// 	size_t	total_args;
// 	size_t	i;

// 	total_args = count_args(ast);
// 	args = (char**)ft_memalloc(sizeof(char*) * (total_args + 1));
// 	if (args == NULL)
// 		return (NULL);
// 	i = 0;
// 	probe = ast;
// 	while (i < total_args)
// 	{
// 		#ifdef DEBUG
// 		if (probe->type != WORD)
// 			ft_putendl("Found non-WORD item in child-flow of WORD's (aka I messed something up -Jorn)");
// 		#endif
// 		args[i] = ft_strdup(probe->value);
// 		if (args[i] == NULL)
// 		{
// 			ft_strarrdel(&args);
// 			return (NULL);
// 		}
// 		probe = probe->child;
// 		i++;
// 	}
// }

/*
**	This will edit the I/O table based on the redirect given as input.
**	DOESNT WORK YET
*/

static void exec_redir(t_ast *node, t_envlst *envlst, int *exit_code)
{
	// t_ast	*probe;
	t_ast	*left;

	(void)exit_code;
	(void)envlst;

	left = node->sibling;
	if (left->type == IO_NUMBER || left->type == WORD)
	{
		if (node->type == SLESS || node->type == DLESS || node->type == LESSAND)
		{
			redir_input(node, exit_code);
		}
		else if (node->type == SGREAT || node->type == DGREAT
			|| node->type == GREATAND)
		{
			redir_output(node, exit_code);
		}
	}
}

static void	exec_assign(t_ast *node, t_envlst *envlst, int *exit_code)
{
	builtin_assign(node->value, envlst, exit_code, ENV_TEMP);
}

/*
**	This is used to handle all the redirects and/or assignments in a
**	complete_command
*/

static void	exec_redirs_or_assigns(t_ast *node, t_envlst *envlst, int *exit_code)
{
	t_ast	*probe;

	probe = node;
	while (probe != NULL)
	{
		if (tool_is_redirect_tk(node->type) == true)
			exec_redir(probe, envlst, exit_code);
		else if (probe->type == ASSIGN)
			exec_assign(probe, envlst, exit_code);
		probe = probe->child;
	}
}

/*
**	This function has to prepare the complete_command before
**	execution. Wildcard, quote removal, variables.
*/

static void	exec_complete_command(t_ast *node, t_envlst *envlst, int *exit_code, int flags)
{
	char	**command;

	int fdstdin;
	int	fdstdout;
	int fdstderr;

	(void)flags;

	fdstdin = dup(STDIN_FILENO);
	fdstdout = dup(STDOUT_FILENO);
	fdstderr = dup(STDERR_FILENO);
	/* Replace wildcards */
	/* Replace variables */

	/* There is atleast one cmd_word in complete_command */
	exec_quote_remove(node);
	if (node->type == WORD)
	{
		if (node->sibling)
			exec_redirs_or_assigns(node->sibling, envlst, exit_code);

		/* Remove useless quotes */
		/* Remove useless escape chars */
		
		command = create_args(node);
		/* add handling of flag = EXEC_PIPE */
		/* add option for flag = EXEC_BG */
		if (command != NULL)
			exec_cmd(command, envlst, exit_code);
	}

	/* There is no cmd_word in complete_command */
	else if (node->type == ASSIGN || tool_is_redirect_tk(node->type) == true)
		exec_redirs_or_assigns(node, envlst, exit_code);
	
	dup2(fdstdin, STDIN_FILENO);
	dup2(fdstdout, STDOUT_FILENO);
	dup2(fdstderr, STDERR_FILENO);
}

// 	t_ast	*probe;
// 	char	*left;
// 	char	*right;

// 	(void)exit_code;
// 	(void)envlst;

// 	probe = node->sibling;
// 	if (probe->type == WORD)
// 		left = probe->value;
// 	else
// 		left = parser_return_token_str(probe->type);
// 	probe = node->sibling->child;
// 	if (probe->type == WORD)
// 		right = probe->value;
// 	else
// 		right = parser_return_token_str(probe->type);
// 	ft_printf("Redirect: %s > %s\n", left, right);
// }

/*
**	General structure:
**	Read PR.
*/

void		exec_start(t_ast *ast, t_envlst *envlst, int *exit_code, int flags)
{
	if (ast == NULL)
		return ;
	/* Set flags */
	if (ast->type == PIPE)
		flags &= ~EXEC_PIPE;
	else if (ast->type == BG)
		flags &= ~EXEC_BG;
	else if (ast->type == AND_IF)
		flags &= ~EXEC_AND_IF;
	else if (ast->type == OR_IF)
		flags &= ~EXEC_OR_IF;
	else if (ast->type == SEMICOL)
		flags &= ~EXEC_SEMICOL;

	/* Goes through the tree to find complete_commands first */
	/* problem if there are no WORD's but only prefix or suffix */
	if (ast->type != WORD && ast->type != ASSIGN && tool_is_redirect_tk(ast->type) == false)
		exec_start(ast->child, envlst, exit_code, flags);
	
	/* Runs after the above exec_start returns or isn't run */
	if (ast->type == AND_IF && *exit_code != EXIT_SUCCESS)
		return ;
	else if (ast->type == WORD || tool_is_redirect_tk(ast->type) == true)
		exec_complete_command(ast, envlst, exit_code, flags);
	else if (ast->sibling != NULL)
		exec_start(ast->sibling, envlst, exit_code, flags);
}
