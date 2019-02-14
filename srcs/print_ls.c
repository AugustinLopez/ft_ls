/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_ls.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lubenard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/12 19:28:08 by lubenard          #+#    #+#             */
/*   Updated: 2019/02/14 11:16:59 by aulopez          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_ls.h>

void	print_ls(t_ls *liste1)
{
	t_stat	*tmp;
	t_file	*tmp2;

	tmp = &(liste1->curr_file->stat);
	tmp2 = liste1->curr_file;
	//if (liste1->curr_file->prev)
	//	ft_printf("Previous file is %s\n", liste1->curr_file->prev->name);
	if (tmp2->name)
	{
		printf("%s\n", tmp2->name);
		if (tmp2->prev)
			printf("Previous :%s\n", tmp2->prev->name);
		printf("NVL ELEMENT :\nSTAT:\n"
		"inode = %lld\nmode = %ho\n"
		"nbr de liens = %hu\n"
		"proprio = %u\n"
		/*"taille de blocs = %i\n"
		"taille de fichier = %lld\n"
		"blocks alloues = %lld\n"
		"next = %p\n"
		"actual = %p\n"
		"previous = %p\n"*/
		"----------------------------------------------------------------\n",
		tmp->st_ino, tmp->st_mode, tmp->st_nlink, tmp->st_uid
		/*tmp->st_gid, tmp->st_blksize, tmp->st_size, tmp->st_blocks, tmp2->next, tmp2, tmp2->prev*/);
	}
}

void	print_previous(t_ls *ls)
{
	t_stat	*stat;
	t_file	*tmp;

	stat = &(ls->curr_file->prev->stat);
	tmp = ls->curr_file->prev;
	if (tmp->name)
	{
		printf("%s\n", tmp->name);
		if (tmp->prev)
			printf("Previous :%s\n", tmp->prev->name);
		printf("NVL ELEMENT :\nSTAT:\n"
		"inode = %lld\nmode = %ho\n"
		"nbr de liens = %hu\n"
		"proprio = %u\n"
		/*"taille de blocs = %i\n"
		"taille de fichier = %lld\n"
		"blocks alloues = %lld\n"
		"next = %p\n"
		"actual = %p\n"
		"previous = %p\n"*/
		"----------------------------------------------------------------\n",
		stat->st_ino, stat->st_mode, stat->st_nlink, stat->st_uid
	/*	stat->st_gid, stat->st_blksize, stat->st_size, stat->st_blocks, tmp->next, tmp, tmp->prev*/);
	}
}

