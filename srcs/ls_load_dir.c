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

int						create_directory_from_arg(t_ls *ls, char *argv)
{
	char		*tmp;
	size_t		i;
	t_list		*tmpdir;

	if (!(tmpdir = ft_lstnew(0, 0)))
		return (ls_print_error(0, LSERR_MALLOC));
	i = ft_strlen(argv);
	if (!(tmp = ft_strnew(i + 1 + 256 + 2)))
		return (ls_print_error(0, LSERR_MALLOC));
	ft_strcat(tmp, ls->directory->pv);
	ft_strcat(tmp + ls->directory->zu, argv);
	//if (argv[i  - 1] != '/')
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

int						create_directory(t_ls *ls)
{
	if (!ls->directory && !ls->file)
		return (create_first_directory(ls, (ls->flags & LSO_ARGC)));
	else
		return (create_another_directory(ls));
}
