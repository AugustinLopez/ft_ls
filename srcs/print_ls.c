/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_ls.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lubenard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/12 19:28:08 by lubenard          #+#    #+#             */
/*   Updated: 2019/02/12 20:02:37 by lubenard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_ls.h>

void	print_ls(t_ls *liste1)
{
	t_stat	*tmp;
	t_file	*tmp2;

	tmp = liste1->curr_file->stat;
	tmp2 = liste1->curr_file;
	while (tmp2)
		ft_printf("NVL ELEMENT :\nSTAT:\ninode = %lld\nmode = %lo\nnbr de liens = %ld\nproprio = %ld %ld\ntaille de blocs = %ld\ntaille de fichier = %ld\nblocks alloues = %lld\nnext = %p\n----------------------------------------------------------------\n", tmp->st_ino, tmp->st_mode, tmp->st_nlink, tmp->st_uid, tmp->st_gid, tmp->st_blksize, tmp->st_size, tmp->st_blocks, tmp2->next);
	tmp2 = tmp2->next;
}
