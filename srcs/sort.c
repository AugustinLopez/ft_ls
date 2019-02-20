/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lubenard <lubenard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/13 22:51:42 by lubenard          #+#    #+#             */
/*   Updated: 2019/02/20 14:39:34 by lubenard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_ls.h>

/*
** to help, see this video : https://www.youtube.com/watch?v=OGzPmgsI-pQ
*/

inline static void		switch_name(t_file *first, t_file *next)
{
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

void					sort_ascii(t_file *current, t_file *last)
{
	t_file	*next;
	t_file	*first;
	t_file	*other;

	other = last->next;
	last->next = NULL;
	first = current;
	while (current->next)
	{
		next = current->next;
		if (ft_strcmp(current->name, next->name) > 0)
		{
			while (first->next && ft_strcmp(first->name, next->name) < 0)
				first = first->next;
			switch_name(first, next);
			current = current->prev;
			while (first->prev)
				first = first->prev;
		}
		current = current->next;
	}
	current->next = other;
	if (other)
		other->prev = current;
}

void					sort_time(t_file *current, t_file *last)
{
	t_file	*next;
	t_file	*first;
	t_file	*other;

	other = last->next;
	last->next = NULL;
	first = current;
	while (current->next)
	{
		next = current->next;
		if (current->stat.st_mtime < next->stat.st_mtime)
		{
			while (first->next && first->stat.st_mtime >= next->stat.st_mtime)
				first = first->next;
			switch_name(first, next);
			current = current->prev;
			while (first->prev)
				first = first->prev;
		}
		current = current->next;
	}
	current->next = other;
	if (other)
		other->prev = current;
}

void					sort_access_time(t_file *current, t_file *last)
{
	t_file	*next;
	t_file	*first;
	t_file	*other;

	other = last->next;
	last->next = NULL;
	first = current;
	while (current->next)
	{
		next = current->next;
		if (current->stat.st_atime < next->stat.st_atime)
		{
			while (first->next && first->stat.st_atime >= next->stat.st_atime)
				first = first->next;
			switch_name(first, next);
			current = current->prev;
			while (first->prev)
				first = first->prev;
		}
		current = current->next;
	}
	current->next = other;
	if (other)
		other->prev = current;
}
