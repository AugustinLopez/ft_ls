/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ls_load_info.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aulopez <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/14 12:09:32 by aulopez           #+#    #+#             */
/*   Updated: 2019/02/18 21:04:01 by lubenard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_ls.h>

inline static int		load_file_link(t_ls *ls)
{
	ls->flags &= ~LSO_1STFILE;
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
		{
			ls->curr_file = ls->curr_file->next;
			if (ls->curr_file->name)
				free(ls->curr_file->name);
		}
	}
	return (1);
}

inline static int		load_file_stats(t_ls *ls, char *filename)
{
	char	*tmp;

	if (!ls->file || !(ls->flags & LSO_1STFILE))
		load_file_link(ls);
	else if (ls->file->name)
		free(ls->file->name);
	tmp = (char*)(ls->directory->pv);
	if (!(ls->curr_file->name = ft_strdup(filename)))
		return (ls_print_error(0, LSERR_MALLOC));
	tmp[ls->directory->zu] = 0;
	if (lstat(ft_strcat(tmp, filename), &(ls->curr_file->stat)) < 0)
		return (ls_print_error(filename, LSERR_OPENFILE));
	/*if ((ls->flags & LSO_RR)
	&& S_ISDIR(ls->curr_file->stat.st_mode)
	&& ft_strcmp(ls->curr_file->name, ".") != 0
	&& ft_strcmp(ls->curr_file->name, "..") != 0
	&& !(create_directory(ls)))
		return (0);*/
	if (ls->flags & LSO_1STFILE)
		ls->flags &= ~LSO_1STFILE;
	return (1);
}

int						load_info_from_directory(t_ls *ls)
{
	DIR			*ddd;
	t_dirent	*dir;

	ls->numfile = 0;
	if ((ddd = opendir((char*)(ls->directory->pv))))
	{
		ls->flags |= LSO_1STFILE;
		ls->curr_file = ls->file;
		while ((dir = readdir(ddd)))
			if ((dir->d_name[0] != '.' || ls->flags & LSO_A) && ++(ls->numfile))
				load_file_stats(ls, &(dir->d_name[0]));
	}
	else
		ls_print_error((char*)(ls->directory->pv), LSERR_OPENDIR);
	if (ddd)
		closedir(ddd);
	((char*)(ls->directory->pv))[ls->directory->zu] = 0;
	return (0);
}

int						next_dir(t_ls *ls)
{
	t_list		*tmp;

	tmp = ls->directory;
	ls->directory = ls->directory->next;
	ls->curr_dir = ls->directory;
	ft_lstdelone(&tmp, *ft_lstfree);
	return (0);
}

int						load_info_from_argument(t_ls *ls, int argc, char **argv)
{
	t_stat		stat;
	int			i;

	i = 0;
	while (i < argc)
	{
		if (ft_strchr("/~", argv[i][0]) || !ft_strncmp(argv[i], "./", 2))
			ft_bzero(ls->directory->pv, 3);
		else
			ft_strcpy(ls->directory->pv, "./");
		if (lstat(argv[i], &stat) < 0)
			ls_print_error(argv[i], LSERR_OPENFILE);
		/*else if (S_ISDIR(stat.st_mode))
			create_directory_from_arg(ls, argv[i]);*/
		else if (++(ls->numfile))
			load_file_stats(ls, argv[i]);
		i++;
	}
	ft_bzero(ls->directory->pv, 3);
	return (1);
}
