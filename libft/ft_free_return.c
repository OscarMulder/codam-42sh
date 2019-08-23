/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_free_return.c                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: tde-jong <tde-jong@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/08/01 14:58:31 by tde-jong       #+#    #+#                */
/*   Updated: 2019/08/23 14:27:15 by tde-jong      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_free_return(void *tofree, int ret)
{
	ft_strdel((char**)&tofree);
	return (ret);
}
