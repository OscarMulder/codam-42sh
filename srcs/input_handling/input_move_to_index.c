/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   input_move_to_index.c                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: rkuijper <rkuijper@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/08/01 14:11:23 by rkuijper       #+#    #+#                */
/*   Updated: 2019/08/03 15:19:16 by rkuijper      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"

/*
**
**	Moves the cursor backward/up (both visually and in the background).
**		- Extra checks for newline handling and first line (prompt space).
**
*/
static void	move_cursor_backward(unsigned *index, unsigned new_index,
    t_vshdata *vshdata)
{
    int amt;
    
    while (*index > new_index)
    {
        (*index)--;
        if (vshdata->line[*index] == '\n')
        {
            amt = *index - ft_strrnchri(vshdata->line, '\n', *index - 1, 0) - 1;
            if (ft_strrnchri(vshdata->line, '\n', *index - 1, 0) == 0)
                amt += vshdata->prompt_len;
            ft_printf("\e[A\e[%dC", amt);
        }
        else
            ft_printf("\e[D");        
    }
}

/*
**
**	Moves the cursor forward/down (both visually and in the background).
**		- No extra checks needed for first line, since we won't encounter
**		  the prompt going down.
**		- Backward motion upon encountering a newline is hacky (10000D)
**		  very hardcoded, rework?
**
*/

static void	move_cursor_forward(unsigned *index, unsigned new_index, char *line)
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


/*
**
**	Moves the cursor to the new_index position.
**	(both visually and programatically/in the background).
**		- Calls either 'move_cursor_backward' or 'move_cursor_forward',
**		  Depending on which direction the cursor should move.
**
*/

void		input_move_to_index(unsigned *index, unsigned new_index,
	t_vshdata *vshdata)
{
    if (new_index <= ft_strlen(vshdata->line))
    {
        if (new_index < *index)
            move_cursor_backward(index, new_index, vshdata);
        else if (new_index > *index)
            move_cursor_forward(index, new_index, vshdata->line);
    }
}
