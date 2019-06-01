/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   exec.h                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: omulder <omulder@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/06/01 12:27:54 by omulder        #+#    #+#                */
/*   Updated: 2019/06/01 12:52:38 by omulder       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXEC_H
# define EXEC_H
# include "vsh.h"

int		exec_start(t_ast *ast, int *exit_code);
int		exec_cmd(char **args, char ***env, int *exit_code);
bool	exec_builtin(char **args, char ***env, int *exit_code);
bool	exec_external(char **args, char ***env, int *exit_code);

#endif
