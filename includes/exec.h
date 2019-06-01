/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   exec.h                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: omulder <omulder@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/06/01 12:27:54 by omulder        #+#    #+#                */
/*   Updated: 2019/06/01 12:29:15 by omulder       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXEC_H
# define EXEC_H
# include "vsh.h"

int		exec_start(t_ast *ast);
int		exec_cmd(char **args, char ***env);
int		exec_builtin(char **args, char ***env);
bool	exec_extern(char **args, char **env, int *status);

#endif
