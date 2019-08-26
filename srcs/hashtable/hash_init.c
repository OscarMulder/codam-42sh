/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   hash_init.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: mavan-he <mavan-he@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/08/19 11:23:37 by mavan-he       #+#    #+#                */
/*   Updated: 2019/08/26 18:40:20 by jbrinksm      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"

void	hash_init(t_vshdata *data)
{
	int i;

	i = 0;
	while (i < HT_SIZE)
	{
		data->hashtable->ht[i] = NULL;
		i++;
	}
	data->hashtable->ht_flag = HT_EMPTY;
}
