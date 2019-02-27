/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ls_load_info_devfd.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aulopez <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/25 15:06:23 by aulopez           #+#    #+#             */
/*   Updated: 2019/02/27 11:25:06 by aulopez          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_ls.h>

int		handle_dev_fd(t_ls *ls, char *pathname)
{
	char	buf[PATH_MAX + 1];
	t_stat	t1;
	t_stat	t2;

	ft_bzero(&buf, PATH_MAX + 1);
	pathname ? ft_strcat(buf, pathname) : 0;
	!ls->directory->zu ? ft_strcpy(buf, "../fd") : ft_strcat(buf, "../fd");
	lstat(buf, &t1);
	lstat("/dev/fd", &t2);
	if (t1.st_ino == t2.st_ino)
	{
		if ((!ls->file || !(ls->flags & LSO_1STFILE)) && (!load_file_link(ls)))
			return (ls_print_error(0, LSERR_MALLOC, ls));
		if (lstat("/dev/fd", &(ls->curr_file->stat)) < 0)
			return (ls_print_error(".", LSERR_OPENFILE, ls));
		ls->numfile++;
		ft_strcpy(ls->curr_file->name, ".");
		if ((!ls->file || !(ls->flags & LSO_1STFILE)) && (!load_file_link(ls)))
			return (ls_print_error(0, LSERR_MALLOC, ls));
		if (lstat("/dev/.", &(ls->curr_file->stat)) < 0)
			return (ls_print_error("..", LSERR_OPENFILE, ls));
		ls->numfile++;
		ft_strcpy(ls->curr_file->name, "..");
	}
	return (1);
}
