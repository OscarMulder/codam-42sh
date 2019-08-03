/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   input_move_to_index.c                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: rkuijper <rkuijper@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/08/01 14:11:23 by rkuijper       #+#    #+#                */
/*   Updated: 2019/08/03 16:27:29 by rkuijper      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vsh.h"

/*
**
**	Moves the cursor backward/up (both visually and in the background).
**		- If no newline is encountered in one step back,
**			the cursor is simply moved one space back `\e[D`.
**		- When moving the cursor backwards and encountering a newline,
**			the length of the string before the newline is calculated,
**			up to the next encountered newline or the beginning of the buffer.
**		- If there is no newline to be found, the length of the prompt is added
**			to the amt variable, since it's moving to the first line of the
**			buffer and should account for the space of the shell prompt.
**		- At last, the cursor is moved one space up `\e[A and 'amt' spaces
**			to the right `\e[%dC` to have the cursor end up at the end of
**			the string on the previous line.
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
**		- If no newline is encountered, the cursor is simply moved one space
**			forward `\e[C`.
**		- If a newline is encountered, the cursor is moved down `\e[B` and
**			10000 spaces backwards, to make sure the cursor ends up at the
**			leftmost edge of the screen, to start from the leftmost side of
**			the line below.
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
