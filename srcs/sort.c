/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lubenard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/12 21:44:06 by lubenard          #+#    #+#             */
/*   Updated: 2019/02/12 22:20:00 by lubenard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include <ft_ls.h>

void	sortedInsert(t_file **head_ref, t_file *new_node)
{
	t_file		*current;

	if (*head_ref == NULL || ft_strcmp((*head_ref)->name, new_node->name) < 0)
	{
		new_node->next = *head_ref;
		*head_ref = new_node;
	}
	else
	{
		current = *head_ref;
		while (current->next != NULL
				&& ft_strcmp(current->next->name, new_node->name) < 0)
			current = current->next;
		new_node->next = current->next;
		current->next = new_node;
	}
}

void	sort(t_ls *lkd_list)
{
	t_file	*sorted;
	t_ls	*current;

	sorted = NULL;
	current = lkd_list;
	while (current != NULL)
	{
		t_file *next = current->curr_file->next;
		sortedInsert(&sorted, current->curr_file);
		current->curr_file = next;
	}
	lkd_list->curr_file = sorted;
	while (lkd_list)
	{
		printf("%s %p\n", lkd_list->curr_file->name, lkd_list->curr_file->next);
		lkd_list->curr_file = lkd_list->curr_file->next;
	}
}
