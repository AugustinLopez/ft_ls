/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lubenard <lubenard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/13 22:51:42 by lubenard          #+#    #+#             */
/*   Updated: 2019/02/18 21:11:36 by lubenard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_ls.h>

/*
** to help, see this video : https://www.youtube.com/watch?v=OGzPmgsI-pQ
*/

void	switch_name(t_file *first, t_file *next)
{
	while (first->next && ft_strcmp(first->name, next->name) < 0)
		first = first->next;
	if (next->prev != NULL)
		next->prev->next = next->next;
	if (next->next != NULL)
		next->next->prev = next->prev;
	if (first->prev != NULL)
		first->prev->next = next;
	next->next = first;
	next->prev = first->prev;
	first->prev = next;
}

void	sort(t_file *current)
{
	t_file	*next;
	t_file	*first;

	first = current;
	while (current->next)
	{
		next = current->next;
		if (ft_strcmp(current->name, next->name) > 0)
		{
			switch_name(first, next);
			current = current->prev;
			while (first->prev)
				first = first->prev;
		}
		current = current->next;
	}
}
