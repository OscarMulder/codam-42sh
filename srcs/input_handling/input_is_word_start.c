/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   input_is_word_start.c                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: rkuijper <rkuijper@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/05/17 14:49:18 by rkuijper       #+#    #+#                */
/*   Updated: 2019/05/17 14:50:38 by rkuijper      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

int			input_is_word_start(char *str, int i1, int i2)
{
	return ((str[i1] == ' ' || str[i1] == '\t') &&
		str[i2] > 32 && str[i2] < 127);
}