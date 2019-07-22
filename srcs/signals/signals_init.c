/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   signals_init.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: tde-jong <tde-jong@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/07/22 13:01:13 by tde-jong       #+#    #+#                */
/*   Updated: 2019/07/22 13:07:00 by tde-jong      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"

void	signals_init(void)
{
	signal(SIGINT, signals_handle);
}
