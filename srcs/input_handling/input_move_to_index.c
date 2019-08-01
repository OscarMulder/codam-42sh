/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   input_move_to_index.c                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: rkuijper <rkuijper@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/08/01 14:11:23 by rkuijper       #+#    #+#                */
/*   Updated: 2019/08/01 16:17:08 by rkuijper      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"

/* TODO
    fix first line issues with shell prompt (saved in vshdata->prompt_len).
*/
static void move_cursor_backward(unsigned *index, unsigned new_index,
    char *line, t_vshdata *vshdata)
{
    int amt;
    
    while (*index > new_index)
    {
        (*index)--;
        if (line[*index] == '\n')
        {
            amt = *index - ft_strrnchri(line, '\n', *index - 1, 0) - 1;
            if (ft_strrnchri(line, '\n', *index - 1, 0) == 0)
                amt += vshdata->prompt_len;
            ft_printf("\e[A\e[%dC", amt);
        }
        else
            ft_printf("\e[D");        
    }
}

static void move_cursor_forward(unsigned *index, unsigned new_index, char *line)
{
    while (*index < new_index)
    {
        if (line[*index] == '\n')
            ft_printf("\e[B\e[10000D");
        else
            ft_printf("\e[C");
        (*index)++;
    }
}

void        input_move_to_index(unsigned *index, unsigned new_index,
    char *line, t_vshdata *vshdata)
{
    if (new_index <= ft_strlen(line))
    {
        if (new_index < *index)
            move_cursor_backward(index, new_index, line, vshdata);
        else if (new_index > *index)
            move_cursor_forward(index, new_index, line);
    }
}
