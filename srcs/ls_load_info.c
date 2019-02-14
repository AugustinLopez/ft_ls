/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ls_load_info.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aulopez <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/14 12:09:32 by aulopez           #+#    #+#             */
/*   Updated: 2019/02/14 13:00:39 by aulopez          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_ls.h>

inline static int		load_file_link(t_ls *ls)
{
	if (ls->directory) //every case except first call argument given
	{
		if (!(ls->file)) //we need to create a link
		{
			if (!(ls->file = malloc(sizeof(t_file))))
				return (ls_print_error(0, LSERR_MALLOC));
			ft_bzero(ls->file, sizeof(t_file));
			ls->curr_file = ls->file;
		}
		else if (!(ls->curr_file->next))
		{
			if (!(ls->curr_file->next = malloc(sizeof(t_file))))
				return (ls_print_error(0, LSERR_MALLOC));
			ft_bzero(ls->curr_file->next, sizeof(t_file));
			ls->curr_file->next->prev = ls->curr_file;
			ls->curr_file = ls->curr_file->next;
			ls->curr_file->next = 0;
		}
		else
			ls->curr_file = ls->curr_file->next;
	}
	return (1);
}

inline static int		load_file_stats(t_ls *ls, char *filename)
{
	char	*tmp;

	load_file_link(ls);
	tmp = (char*)(ls->directory->pv);
	if (!(ls->curr_file->name = ft_strdup(filename)))
		return (ls_print_error(0, LSERR_MALLOC));
	if (lstat(ft_strcat(tmp, filename), &(ls->curr_file->stat)) < 0) 
		return (-1);
	tmp[ls->directory->zu] = 0;
	if (S_ISDIR(ls->curr_file->stat.st_mode))
	{
		if (ft_strcmp(ls->curr_file->name, ".") != 0
		&& ft_strcmp(ls->curr_file->name, "..") != 0)
			load_directory(ls);
	}
	return (0);
}

int						load_info_from_directory(t_ls *ls)
{
	DIR			*ddd;
	t_dirent	*dir;
	t_list		*tmp;

	if ((ddd = opendir((char*)(ls->directory->pv))))
	{
		while ((dir = readdir(ddd)))
			if (dir->d_name[0] != '.' || ls->flags & LSO_A)
				load_file_stats(ls, &(*dir->d_name));
	}
		else
			return (ls_print_error((char*)(ls->directory->pv), LSERR_OPENDIR));
	if (ddd)
		closedir(ddd);
	tmp = ls->directory;
	ls->directory = ls->directory->next;
	ft_lstdelone(&tmp, *ft_lstfree);
	return (0);
}
