/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_ls.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lubenard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/12 19:28:08 by lubenard          #+#    #+#             */
/*   Updated: 2019/02/14 12:34:34 by aulopez          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_ls.h>

void	print_ls(t_ls *liste1)
{
	t_stat	*tmp;
	t_file	*tmp2;

	tmp = &(liste1->curr_file->stat);
	tmp2 = liste1->curr_file;
	if (tmp2->name)
	{
		ft_printf("%s\n", tmp2->name);
		if (tmp2->prev)
			ft_printf("Previous :%s\n", tmp2->prev->name);
		ft_printf("NVL ELEMENT :\nSTAT:\n"
		"inode = %lld\nmode = %ho\n"
		"nbr de liens = %hu\n"
		"proprio = %u %u\n"
		"taille de blocs = %i\n"
		"taille de fichier = %lld\n"
		"blocks alloues = %lld\n"
		"next = %p\n"
		"actual = %p\n"
		"previous = %p\n"
		"----------------------------------------------------------------\n",
		tmp->st_ino, tmp->st_mode, tmp->st_nlink, tmp->st_uid,
		tmp->st_gid, tmp->st_blksize, tmp->st_size, tmp->st_blocks, tmp2->next, tmp2, tmp2->prev);
	}
}