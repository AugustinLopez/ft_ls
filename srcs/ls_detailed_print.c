/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ls_detailed_print.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aulopez <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/18 14:19:19 by aulopez           #+#    #+#             */
/*   Updated: 2019/02/28 10:47:09 by aulopez          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_ls.h>

/*
** 0-4 are block/link/user/group/size length
** 5-6 are major/minor
** 7 is total block
** 8 is used to know if there are major/minor
** 9 is a temp variable
** if 10/11,no name associated to a uid/gid : we use the ID instead
*/

inline static void	print_date(t_file *tmp, t_ls *ls)
{
	char	*t;

	t = ctime(&tmp->stat.st_mtime) + 4;
	if (ls->flags & LSO_TT)
	{
		if ((t + 16)[0] != ' ')
			ft_printf("%.20s ", t);
		else
			ft_printf("%.15s %.5s ", t, t + 20);
	}
	else if (time(NULL) > LS_SIX_MONTH && tmp->stat.st_mtime <= time(NULL)
	&& tmp->stat.st_mtime > time(NULL) - LS_SIX_MONTH)
		ft_printf("%.12s ", t);
	else if ((t + 16)[0] != ' ')
		ft_printf("%.6s  %.4s ", t, t + 16);
	else
		ft_printf("%.6s  %.5s ", t, t + 20);
}

inline static void	print_detailed_loop_2(t_ls *ls, t_file *tmp,
		long long (*s)[12], char (*attr)[12])
{
	ls->flags & LSO_S ? ft_printf("%*lld ", (*s)[0], tmp->stat.st_blocks) : 0;
	ft_printf("%s %*lld ", *attr, (*s)[1], tmp->stat.st_nlink);
	if (!(ls->flags & LSO_G))
		!(*s)[10] || getpwuid(tmp->stat.st_uid) ?
			ft_printf("%-*s  ", (*s)[2], getpwuid(tmp->stat.st_uid)->pw_name) :
			ft_printf("%-*lld  ", (*s)[2], tmp->stat.st_uid);
	if (!(ls->flags & LSO_O))
		!(*s)[11] || getgrgid(tmp->stat.st_gid) ?
			ft_printf("%-*s  ", (*s)[3], getgrgid(tmp->stat.st_gid)->gr_name) :
			ft_printf("%-*lld  ", (*s)[3], tmp->stat.st_gid);
	((*s)[8] && ((*attr)[0] == 'b' || (*attr)[0] == 'c')) ?
		ft_printf("%*lld, %*lld ", (*s)[5], major(tmp->stat.st_rdev),
		(*s)[6], minor(tmp->stat.st_rdev)) :
		ft_printf("%*lld ", (*s)[4], tmp->stat.st_size);
	print_date(tmp, ls);
	set_colors(tmp, ls);
}

inline static void	print_link(char *path)
{
	char	buf[PATH_MAX + 1];

	ft_bzero(buf, PATH_MAX + 1);
	readlink(path, buf, PATH_MAX);
	ft_printf(" -> %s", buf);
}

void				print_detailed_loop(t_ls *ls, long long (*s)[12])
{
	t_file		*tmp;
	char		attr[12];

	tmp = (ls->flags & LSO_R) ? ls->curr_file : ls->file;
	while (tmp)
	{
		if ((ls->flags & LSO_AA)
			&& (!ft_strcmp(tmp->name, ".") || !ft_strcmp(tmp->name, "..")))
		{
			tmp = (ls->flags & LSO_R) ? tmp->prev : tmp->next;
			continue ;
		}
		if (ls->numfile-- == 0)
			break ;
		((char*)(ls->directory->pv))[ls->directory->zu] = 0;
		ft_strcat(ls->directory->pv, tmp->name);
		load_attribute(tmp, ls, &attr);
		print_detailed_loop_2(ls, tmp, s, &attr);
		attr[0] == 'l' ? print_link((char*)ls->directory->pv) : 0;
		ft_putchar('\n');
		((char*)(ls->directory->pv))[ls->directory->zu] = 0;
		tmp = (ls->flags & LSO_R) ? tmp->prev : tmp->next;
	}
}

void				print_detailed(t_ls *ls, int non_first)
{
	long long	size[12];

	ft_bzero(size, 12 * (sizeof(long long)));
	set_detailed_list_length(ls, &size);
	if (ls->directory->zu)
		((char*)ls->directory->pv)[ls->directory->zu - 1] = 0;
	if (non_first && !(ls->flags & LSO_ERROPEN))
		ft_printf("%s:\n", ls->directory->pv);
	if (ls->directory->zu)
		((char*)ls->directory->pv)[ls->directory->zu - 1] = '/';
	if (ls->numfile && ls->file && ls->directory->zu)
		ft_printf("total %d\n", (int)size[7]);
	print_detailed_loop(ls, &size);
	if (ls->flags & (LSO_ARGC | LSO_RR))
		if (ls->directory->next && !(!ls->directory->zu && !ls->file))
			ft_putchar('\n');
}
