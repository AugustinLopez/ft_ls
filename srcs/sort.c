/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lubenard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/13 22:51:42 by lubenard          #+#    #+#             */
/*   Updated: 2019/02/13 23:12:24 by lubenard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_ls.h>

void	sort2(t_ls *lkd_list)
{
	t_file	*current;
	t_file	*next;

	printf("Jusque la ca segault pas\n");
	current = lkd_list->curr_file;
	next = current->next;
	//printf("cur = %s next = %s\n", current->name, next->name);
	//while (current != NULL)
	//{
		//	if (ft_strcmp(current->name, next->name) < 0)
	//	current = current->next;
	//}
}
