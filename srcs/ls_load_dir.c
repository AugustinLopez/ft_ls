/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   load_dir.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lubenard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/12 20:43:00 by lubenard          #+#    #+#             */
/*   Updated: 2019/02/18 21:27:56 by lubenard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_ls.h>

inline static int		create_first_directory(t_ls *ls, int ac)
{
	char		*tmp;
	t_list		*tmpdir;

	if (!(tmpdir = ft_lstnew(0, 0)))
		return (ls_print_error(0, LSERR_MALLOC));
	if (!(tmp = ft_strnew(2 + 256)))
		return (ls_print_error(0, LSERR_MALLOC));
	if (ac)
		tmpdir->zu = 0;
	else
	{
		ft_strcat(tmp, "./");
		tmp[2] = 0;
		tmpdir->zu = 2;
	}
	tmpdir->pv = tmp;
	ls->directory = tmpdir;
	ls->curr_dir = tmpdir;
	return (1);
}

inline static int		create_another_directory(t_ls *ls)
{
	size_t		i;
	t_list		*tmpdir;
	char		*tmpstr;

	if (!(tmpdir = ft_lstnew(0, 0)))
		return (ls_print_error(0, LSERR_MALLOC));
	i = ft_strlen(ls->curr_file->name);
	if (!(tmpstr = ft_strnew(i + 1 + 256 + ls->directory->zu)))
		return (ls_print_error(0, LSERR_MALLOC));
	ft_strcat(tmpstr, ls->directory->pv);
	ft_strcat(tmpstr + ls->directory->zu, ls->curr_file->name);
	tmpstr[i + ls->directory->zu] = 0;
	ft_strcat(tmpstr + ls->directory->zu + i, "/");
	tmpdir->pv = tmpstr;
	tmpdir->zu = i + 1 + ls->directory->zu;
	if (!ls->curr_dir)
		ls->curr_dir = tmpdir;
	else if (!ls->curr_dir->next)
		ls->curr_dir->next = tmpdir;
	else
		ft_lstadd(&(ls->curr_dir->next), tmpdir);
	ls->curr_dir = ls->curr_dir->next;
	return (1);
}

int						create_directory_from_arg(t_ls *ls)
{
	char		*tmp;
	size_t		i;
	t_list		*tmpdir;

	if (!(tmpdir = ft_lstnew(0, 0)))
		return (ls_print_error(0, LSERR_MALLOC));
	i = ft_strlen(ls->curr_file->name);
	if (!(tmp = ft_strnew(i + 1 + 256 + 2)))
		return (ls_print_error(0, LSERR_MALLOC));
	ft_strcat(tmp, ls->directory->pv);
	ft_strcat(tmp + ls->directory->zu, ls->curr_file->name);
	ft_strcat(tmp + ls->directory->zu + i++, "/");
	tmp[i + 1 + ls->directory->zu] = 0;
	tmpdir->pv = tmp;
	tmpdir->zu = i + ls->directory->zu;
	if (!ls->curr_dir)
		ls->curr_dir = tmpdir;
	else if (!ls->curr_dir->next)
		ls->curr_dir->next = tmpdir;
	else
		ft_lstadd(&(ls->curr_dir->next), tmpdir);
	ls->curr_dir = ls->curr_dir->next;
	return (1);
}

int						create_directory(t_ls *ls, int *argc)
{
	unsigned int	i;
	t_file			*tmp;

	tmp = NULL;
	i = 0;
	if (!ls->directory && !ls->file)
		return (create_first_directory(ls, (ls->flags & LSO_ARGC)));
	else if (*argc)
	{
		ls->curr_file = ls->file;
		while (ls->curr_file && i++ < ls->numfile)
		{
			tmp = ls->curr_file->next;
			if (S_ISDIR(ls->curr_file->stat.st_mode))
			{
				create_directory_from_arg(ls);
				if (ls->curr_file->next)
					ls->curr_file->next->prev = ls->curr_file->prev;
				if (ls->curr_file->prev)
					ls->curr_file->prev->next = ls->curr_file->next;
				else
					ls->file = ls->curr_file->next;
				free(ls->curr_file->name);
				free(ls->curr_file);
			}
			ls->curr_file = tmp;
		}
	}
	else if (ls->flags & LSO_RR)
	{
		ls->curr_file = ls->file;
		while (ls->curr_file && i++ < ls->numfile)
		{
			if (S_ISDIR(ls->curr_file->stat.st_mode)
			&& ft_strcmp(ls->curr_file->name, ".")
			&& ft_strcmp(ls->curr_file->name, ".."))
				create_another_directory(ls);
			ls->curr_file = ls->curr_file->next;
		}
	}
	return (1);
}
