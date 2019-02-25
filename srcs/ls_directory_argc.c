/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ls_directory_argc.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aulopez <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/25 15:02:07 by aulopez           #+#    #+#             */
/*   Updated: 2019/02/25 16:58:04 by aulopez          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_ls.h>

inline static int			create_directory_from_arg_unit(t_ls *ls)
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

inline static void			adjust_pointer(t_ls *ls)
{
	if (ls->curr_file->next)
		ls->curr_file->next->prev = ls->curr_file->prev;
	if (ls->curr_file->prev)
		ls->curr_file->prev->next = ls->curr_file->next;
	else
		ls->file = ls->file->next;
}

inline static void			handle_temp(t_ls *ls, t_file **start,
							int option)
{
	t_file *tmp;

	if (!option)
	{
		if (!*start)
		{
			*start = ls->curr_file;
			(*start)->next = NULL;
		}
		else
		{
			ls->curr_file->next = *start;
			(*start) = ls->curr_file;
		}
	}
	else
	{
		while (*start)
		{
			tmp = *start;
			*start = (*start)->next;
			free(tmp);
		}
	}
}

void						create_directory_from_arg_loop(t_ls *ls)
{
	unsigned int	i;
	t_file			*tmp;
	t_file			*tmp2;
	t_file			*tmp_start;

	i = 0;
	tmp_start = NULL;
	tmp = ls->curr_file;
	ls->curr_file = (ls->flags & LSO_R) ? ls->curr_file : ls->file;
	while (ls->curr_file && i++ < ls->numfile)
	{
		tmp2 = (ls->flags & LSO_R) ?
			ls->curr_file->prev : ls->curr_file->next;
		if (S_ISDIR(ls->curr_file->stat.st_mode))
		{
			create_directory_from_arg_unit(ls);
			adjust_pointer(ls);
			handle_temp(ls, &tmp_start, 0);
			ls->numfile--;
			i--;
		}
		ls->curr_file = tmp2;
	}
	ls->curr_file = tmp;
	handle_temp(ls, &tmp_start, 1);
}
